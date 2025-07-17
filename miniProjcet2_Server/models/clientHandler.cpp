#include "clientHandler.h"
#include <QThread>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "servermanager.h"
#include "usermanage.h"

ClientHandler::ClientHandler(QTcpSocket* socket, QList<ClientHandler*>* clientList, QObject* parent)
    : QObject(parent), socket(socket), clientList(clientList) {}

void ClientHandler::onReadyRead() {
    qDebug() << "신호 받음";
    QByteArray recvData = socket->readAll();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(recvData, &err);

    if (err.error != QJsonParseError::NoError) {
        qDebug() << "클라이언트에서 받은 정보 Json 파싱 에러 발생";
        return;
    }

    userManage usermanage;
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        QString type = obj.value("type").toString();

        if (type == "login") {
            QString ID = obj.value("ID").toString();
            QString PW = obj.value("PW").toString();
            QString nameOut;
            bool val = usermanage.signIn(ID, PW, nameOut);

            QJsonObject JsonResponse;
            JsonResponse["type"] = "response";
            JsonResponse["result"] = val ? "success" : "fail";
            JsonResponse["name"] = nameOut;
            if (val) { // 로그인 성공 시에만 리스트에 추가
                if (!clientList->contains(this)) { // 이미 추가되어 있지 않은지 확인 (중복 방지)
                    clientList->append(this);
                    qDebug() << "로그인 성공! 클라이언트 리스트에 추가됨. 현재 클라이언트 수:" << clientList->size();
                }
            }
            QJsonDocument respDoc(JsonResponse);
            QByteArray respData = respDoc.toJson(QJsonDocument::Compact);
            respData.append('\n');
            socket->write(respData);
        }
        else if (type == "signup") {
            QString name = obj.value("Name").toString();
            QString id = obj.value("ID").toString();
            QString pw = obj.value("PW").toString();
            QString phone = obj.value("Phone").toString();

            userInfo info;
            info.name = name;
            info.ID = id;
            info.PW = pw;
            info.phoneNum = phone;

            usermanage.signUp(info);
        }
        else if (type == "messagesend") {
            qDebug() << "message 전달받음";
            QString senderName = obj.value("senderName").toString();
            QString chatViewName = obj.value("chatViewName").toString();
            QString text = obj.value("textMessage").toString();
            QString sendString = "[" + senderName + "] : " + text;

            QJsonObject sendObj;
            sendObj["type"] = "messagesend";
            sendObj["textMessage"] = sendString;
            sendObj["chatViewName"] = chatViewName;
            QJsonDocument sendDoc(sendObj);
            QByteArray sending = sendDoc.toJson(QJsonDocument::Compact);
            sending.append('\n');

            for (ClientHandler* handler : qAsConst(*clientList)) {
                QMetaObject::invokeMethod(handler, "sendMessageToClient", Qt::QueuedConnection, Q_ARG(QByteArray, sending));
                qDebug() << handler->socket <<" 로 전송";
            }
        }
    }
}

void ClientHandler::onDisconnected() {
    qDebug() << "클라이언트 연결 해제";
    socket->deleteLater();
    this->deleteLater();
}

void ClientHandler::sendMessageToClient(QByteArray data) {
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(data);
        socket->flush();
    }
}
