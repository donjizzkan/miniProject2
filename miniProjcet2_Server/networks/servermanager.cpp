#include "servermanager.h"
#include <QNetworkInterface>
#include <QDebug>
ServerManager& ServerManager::getInstance(){
    static ServerManager instance;
    return instance;
}
ServerManager::ServerManager(QObject* parent): QObject(parent){
    run();
}

void ServerManager::run() {

    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection,this, &ServerManager::clientConnect);

    if (!tcpServer->listen(QHostAddress::Any, 51234)) {
        qDebug() << "서버 시작 실패";
        return;
    }

    QString myIP = getMyIP();
    QString serverInfo = QString("%1:%2").arg(myIP).arg(tcpServer->serverPort());
    qDebug() << "Server started on:" << serverInfo;
}

QString ServerManager::getMyIP() {
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : interfaces) {
        if (interface.flags() & QNetworkInterface::IsLoopBack ||
            !(interface.flags() & QNetworkInterface::IsUp) ||
            !(interface.flags() & QNetworkInterface::IsRunning)) {
            continue;
        }
        const QList<QNetworkAddressEntry> addressEntries = interface.addressEntries();
        for (const QNetworkAddressEntry &entry : addressEntries) {
            QHostAddress ipAddress = entry.ip();
            if (ipAddress.protocol() == QAbstractSocket::IPv4Protocol &&
                ipAddress != QHostAddress::LocalHost) {
                return ipAddress.toString();
            }
        }
    }
    return "127.0.0.1";
}

void ServerManager::clientConnect() {
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    if(!clientSocket){
        qDebug() << "새 클라이언트 소켓 연결 실패";
        return;
    }

    QThread *thread = new QThread;

    ClientSetup* setup = new ClientSetup(clientSocket);
    setup->moveToThread(thread);

    connect(setup, &ClientSetup::socketReady, this, [=](QTcpSocket* socket){
        ClientHandler* handler = new ClientHandler(socket);
        connect(socket, &QTcpSocket::readyRead, handler, &ClientHandler::onReadyRead);
        connect(handler,&ClientHandler::disconnected,this,&ServerManager::removeClient);
        connect(thread,&QThread::finished,handler,&QObject::deleteLater);
        connect(thread, &QThread::finished,socket,&QObject::deleteLater);
    });
    connect(thread, &QThread::started, setup, &ClientSetup::start);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::finished, setup, &QObject::deleteLater);

    thread->start();
}

void ServerManager::addClient(ClientHandler* handler){
    clientListMutex.lock();
    if(!clientHandlerList.contains(handler)){
        clientHandlerList.append(handler);
    }
    clientListMutex.unlock();
}

void ServerManager::removeClient(ClientHandler* handler){
    clientListMutex.lock();
    clientHandlerList.removeOne(handler);
    clientListMutex.unlock();
}

void ServerManager::broadcastMessage(QByteArray& data){
    QList<ClientHandler*> toBroadcast;
    clientListMutex.lock();
    toBroadcast = clientHandlerList;
    clientListMutex.unlock();

    for(ClientHandler* handler : toBroadcast){
        QMetaObject::invokeMethod(handler,"sendMessageToClient",Qt::QueuedConnection,Q_ARG(QByteArray,data));
    }
}
