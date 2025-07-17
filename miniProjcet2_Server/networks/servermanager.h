#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QList>
#include "clientHandler.h"
#include "clientsetup.h"
#include <QMutex>

class ServerManager : public QObject {
    Q_OBJECT
public:
    ServerManager();
    void run();
    QString getMyIP();

public slots:
    static ServerManager& getInstance();
    void addClient(ClientHandler* handler);
    void removeClient(ClientHandler* handler);
    void broadcastMessage(QByteArray& data);
    void clientConnect();

private:
    QTcpServer* tcpServer;
    QList<ClientHandler*>* clientHandlerList;
    QList<ClientSetup*>* setupList;
    // 생성자
    ServerManager(QObject* parent = nullptr);
    QMutex clientListMutex;
};

#endif // SERVERMANAGER_H
