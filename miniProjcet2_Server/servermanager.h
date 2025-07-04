#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QMessageBox>

#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QHostAddress>

#define PORT_NUM  51234

class ServerManager : public QObject
{
    Q_OBJECT
public:
    ServerManager();

private:
    void run();
    QTcpServer *tcpServer;
    QList<QTcpSocket*> *clientSocketList;
    QString getMyIP();

private slots:
    void clientConnect();
};

#endif // SERVERMANAGER_H
