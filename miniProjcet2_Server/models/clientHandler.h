#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QList>
#include "usermanage.h"

class ClientHandler : public QObject {
    Q_OBJECT

public:
    explicit ClientHandler(QTcpSocket* socket, QObject* parent = nullptr);

signals:
    void loggedIn(ClientHandler* handler);
    void disconnected(ClientHandler* handler);

public slots:
    void onReadyRead();
    void onDisconnected();
    void sendMessageToClient(QByteArray data);


private:
    QTcpSocket* socket;
    qintptr socketDescriptor;
    QByteArray readBuffer;
    userManage* usermanage;

};

#endif // CLIENTHANDLER_H
