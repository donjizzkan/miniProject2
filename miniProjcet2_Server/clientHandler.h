#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QList>

class ClientHandler : public QObject {
    Q_OBJECT
public:
    explicit ClientHandler(qintptr socketDescriptor, QList<ClientHandler*>* handlerList, QObject* parent = nullptr);

public slots:
    void start();
    void onReadyRead();
    void onDisconnected();
    void sendMessageToClient(QByteArray data);

private:
    QTcpSocket* socket;
    qintptr socketDescriptor;
    QList<ClientHandler*>* clientList;
};

#endif // CLIENTHANDLER_H
