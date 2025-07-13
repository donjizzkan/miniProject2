#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QList>

class ClientHandler : public QObject {
    Q_OBJECT
public:
    ClientHandler(qintptr socketDescriptor, QList<QTcpSocket*> *clientList, QObject* parent = nullptr);
public slots:
    void start();
    void onReadyRead();
    void onDisconnected();
private:
    QTcpSocket* socket;
    qintptr socketDescriptor;
    QList<QTcpSocket*> *clientList;
};

#endif // CLIENTHANDLER_H
