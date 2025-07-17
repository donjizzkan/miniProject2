#include "clientsetup.h"
#include "clientHandler.h"
#include <QThread>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

ClientSetup::ClientSetup(QTcpSocket* sock, QObject* parent)
    : QObject(parent), socket(sock) {}

void ClientSetup::start() {
    socket = new QTcpSocket();
    if (!socket) {
        qDebug() << "소켓 디스크립터 설정 실패";
        return;
    }
    socket->moveToThread(this->thread());
    emit socketReady(socket);
    qDebug() << "클라이언트 스레드에서 소켓 세팅 완료";
}

