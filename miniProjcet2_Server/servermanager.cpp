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
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    qintptr descriptor = clientSocket->socketDescriptor();
    //clientSocket->deleteLater();

    QThread *thread = new QThread;
    ClientHandler *handler = new ClientHandler(descriptor, clientHandlerList);
    handler->moveToThread(thread);

    connect(thread, &QThread::started, handler, &ClientHandler::start);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::finished, handler, &QObject::deleteLater);

    thread->start();
}
