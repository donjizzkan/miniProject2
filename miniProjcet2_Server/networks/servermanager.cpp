#include "servermanager.h"
#include <QNetworkInterface>
#include <QDebug>

ServerManager::ServerManager() {
    run();
}

void ServerManager::run() {
    clientHandlerList = new QList<ClientHandler*>();

    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), SLOT(clientConnect()));

    if (!tcpServer->listen(QHostAddress::Any, 51234)) {
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
    //QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    qintptr descriptor = tcpServer->nextPendingConnection()->socketDescriptor();
    //clientSocket->deleteLater();

    QThread *thread = new QThread;
    ClientSetup* setup = new ClientSetup(descriptor, setupList);
    setup->moveToThread(thread);

    connect(setup, &ClientSetup::socketReady, this, [=](QTcpSocket* socket){
        ClientHandler* handler = new ClientHandler(socket, clientHandlerList);
        connect(socket, &QTcpSocket::readyRead, handler, &ClientHandler::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, handler, &ClientHandler::onDisconnected);
    });
    connect(thread, &QThread::started, setup, &ClientSetup::start);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::finished, setup, &QObject::deleteLater);

    thread->start();
}

void ServerManager::addClient(ClientHandler* handler){
    clientListMutex.lock();
    if(!clientHandlerList->contains(handler)){
        clientHandlerList->append(handler);
    }
    clientListMutex.unlock();
}

void ServerManager::removeClient(ClientHandler* handler){
    clientListMutex.lock();
    clientHandlerList->removeOne(handler);
    clientListMutex.unlock();
}

void ServerManager::broadcastMessage(QByteArray& data){

}
