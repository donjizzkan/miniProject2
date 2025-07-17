#include "clientsetup.h"
#include "clientHandler.h"
#include <QThread>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

ClientSetup::ClientSetup(qintptr socketDescriptor, QList<ClientSetup*>* handlerList, QObject* parent)
    : QObject(parent), socket(nullptr), socketDescriptor(socketDescriptor), clientList(handlerList) {}

void ClientSetup::start() {
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "소켓 디스크립터 설정 실패";
        return;
    }
    emit socketReady(socket);
    qDebug() << "클라이언트 스레드에서 소켓 세팅 완료";
}

