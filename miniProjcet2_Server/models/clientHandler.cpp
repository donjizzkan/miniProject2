#include "clientHandler.h"
#include "servermanager.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>

ClientHandler::ClientHandler(QTcpSocket *socket, QObject *parent)
    : QObject(parent), socket(socket) {
    usermanage = new userManage(this);
    connect(socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);
}

void ClientHandler::onDisconnected() {
    qDebug() << "클라이언트 연결 해제";
    emit disconnected(this);
    socket->deleteLater();
    this->deleteLater();
}

//==========================
//       받은 신호 처리
//==========================
void ClientHandler::onReadyRead() {
    qDebug() << "신호 받음";
    readBuffer.append(socket->readAll());

    while (readBuffer.contains('\n')) {
        int newlineIndex = readBuffer.indexOf('\n');
        QByteArray line = readBuffer.left(newlineIndex);
        readBuffer.remove(0, newlineIndex + 1);

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error != QJsonParseError::NoError) {
            qDebug() << "클라이언트에서 받은 정보 Json 파싱 에러 발생";
            continue;
        }

        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            QString type = obj.value("type").toString();    // 신호의 type 읽어오기

            //==========================
            //       로그인 요청 처리
            //==========================
            if (type == "login") {
                QString ID = obj.value("ID").toString();
                QString PW = obj.value("PW").toString();
                QString nameOut;

                bool val = usermanage->signIn(ID, PW, nameOut);

                QJsonObject JsonResponse;
                JsonResponse["type"] = "response";
                JsonResponse["result"] = val ? "success" : "fail";
                JsonResponse["name"] = nameOut;

                if (val) {
                    // 로그인 성공 시 servermanager 싱글턴 인스턴스에 로그인한 clienthandler 추가 요청
                    ServerManager::getInstance().addClient(this);
                    emit loggedIn(this);
                    qDebug() << "로그인 성공, 클라이언트 리스트에 추가";
                }

                QJsonDocument respDoc(JsonResponse);
                QByteArray respData = respDoc.toJson(QJsonDocument::Compact);
                respData.append('\n');
                socket->write(respData);
            //==========================
            //       회원가입 처리
            //==========================
            } else if (type == "signup") {
                QString name = obj.value("Name").toString();
                QString id = obj.value("ID").toString();
                QString pw = obj.value("PW").toString();
                QString phone = obj.value("Phone").toString();

                userInfo info;
                info.name = name;
                info.ID = id;
                info.PW = pw;
                info.phoneNum = phone;

                usermanage->signUp(info);
            //==========================
            //       메세지 전송 처리
            //==========================
            } else if (type == "messagesend") {
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

                ServerManager::getInstance().broadcastMessage(sending);
                qDebug() << "클라이언트로 전송 요청 완료";

                // 전송 내용 파일로 저장, DB 폴더에 "채팅방 이름"의 json파일을 생성 및 열람
                QFile file("../../DB/" + chatViewName + ".json");

                // 파일이 없을 경우 생성
                if (!file.exists()) {
                    if (file.open(QIODevice::WriteOnly)) {
                        QJsonArray emptyArray;
                        QJsonDocument doc(emptyArray);
                        file.write(doc.toJson());
                        file.close();
                        qDebug() << chatViewName <<" Json 파일 생성";
                    } else {
                        qWarning() << "파일 생성 실패";
                        return;
                    }
                }

                // 저장할 내용 이어붙이기 위해 파일의 앞내용 읽어옴
                QJsonArray messageArray;                   // Json파일내용물 저장될 JsonArray
                if(file.open(QIODevice::ReadOnly)){
                    QByteArray readData = file.readAll();
                    QJsonDocument dataDoc = QJsonDocument::fromJson(readData);
                    messageArray = dataDoc.array();
                    file.close();
                }
                else{
                    qWarning()<<"파일 읽기 실패";
                    return;
                }

                messageArray.append(sendString);      // json Object를 userArray로
                if(file.open(QIODevice::WriteOnly)){
                    QJsonDocument newDoc(messageArray);
                    file.write(newDoc.toJson());
                    file.close();
                    qDebug()<<"메세지 저장 완료";
                }

            //==========================
            //       파일 전송 처리
            //==========================
            } else if (type == "filesend") {
                qDebug() << "file 전달받음";
            }
        }
    }
}

void ClientHandler::sendMessageToClient(QByteArray data) {
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(data);
        socket->flush();
    }
}

