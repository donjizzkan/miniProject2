#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QDebug>
#include "servermanager.h"

// 클라이언트와 연결될 때 멀티 스레드 처리
// 간단한 기능이므로 헤더파일 없이, cpp파일로만 구현

class ClientHandler : public QObject{
    Q_OBJECT
public:
    ClientHandler(QTcpSocket* socket) : socket(socket){}

public slots:
    // 클라이언트 시작
    void start(){
        connect(socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);
    }

    void onReadyRead(){
        QByteArray data = socket->readAll();
        // 데이터 받앗을 때의 처리

        qDebug() << "데이터 받음";
    }

    void onDisconnected(){
        qDebug() << "클라이언트 연결 해제";
        socket->deleteLater();
        this->deleteLater();
    }

private:
    QTcpSocket* socket;
};
