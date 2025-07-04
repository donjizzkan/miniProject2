#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>

class ClientHandler : public QObject {
    Q_OBJECT
public:
    ClientHandler(QTcpSocket* socket, QObject* parent = nullptr);
public slots:
    void start();
    void onReadyRead();
    void onDisconnected();
private:
    QTcpSocket* socket;
};

#endif // CLIENTHANDLER_H
