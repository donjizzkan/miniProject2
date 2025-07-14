#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QList>
#include "clientHandler.h"

class ServerManager : public QObject {
    Q_OBJECT
public:
    ServerManager();
    void run();
    QString getMyIP();

public slots:
    void clientConnect();

private:
    QTcpServer* tcpServer;
    QList<ClientHandler*>* clientHandlerList;
};

#endif // SERVERMANAGER_H
