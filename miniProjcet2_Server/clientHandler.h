#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QDebug>

// 클라이언트와 연결될 때 멀티 스레드 처리
class ClientHandler : public QObject{
    Q_Object;
public:
    ClientHander(QTcpSocket* socket) : socket(socket){}
};

#endif // CLIENTHANDLER_H
