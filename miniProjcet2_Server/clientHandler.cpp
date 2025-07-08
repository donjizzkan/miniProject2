#include "clientHandler.h"
#include <QThread>
#include <QDebug>
#include <QJsonDocument>    // Json관련 라이브러리
#include <QJsonArray>       // Json관련 라이브러리
#include <QJsonObject>      // Json관련 라이브러리
#include "servermanager.h"
#include "usermanage.h"

// 클라이언트와 연결될 때 멀티 스레드 처리

ClientHandler::ClientHandler(QTcpSocket* socket, QObject* parent)
    : QObject(parent), socket(socket) {}
// 클라이언트 시작
void ClientHandler::start() {

    qDebug() <<"클라이언트 시작";
    // 데이터 수신 시 onReadyRead 호출
    connect(socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
    // 연결 해제 시 onDisconnected 호출
    connect(socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);
}

// 데이터 수신 처리
void ClientHandler::onReadyRead() {
    // 클라이언트에서 정보 읽어오기 - donjizzkan
    qDebug() << "신호 받음";
    QByteArray recvData = socket->readAll();   // 소켓에서 받은 Data 변수
    QJsonParseError err;                       // error 날 경우 저장할 변수
    QJsonDocument doc = QJsonDocument::fromJson(recvData, &err); // 소켓에서 받은 Data를 document로 변환하고 실패시 &err에 저장

    if (err.error != QJsonParseError::NoError) {
        qDebug() << "클라이언트에서 받은 정보 Json 파싱 에러 발생";
        return;
    }

    // ==============================
    //          type별 대응
    // ==============================
    userManage usermanage;
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        QString type = obj.value("type").toString(); // type이 뭘로 정의되어있는지 읽어옴

        // 로그인 요청일 때
        if (type == "login") {
            QString ID = obj.value("ID").toString();
            QString PW = obj.value("PW").toString();

            // 리스트에 회원이 있을 경우 true, 없을 경우 false 반환
            bool val = usermanage.signIn(ID, PW);

            QJsonObject JsonResponse;
            JsonResponse["type"] = "response";                // type은 "response"
            JsonResponse["result"] = val ? "success" : "fail";// 결과 대입
            QJsonDocument respDoc(JsonResponse);
            QByteArray respData = respDoc.toJson(QJsonDocument::Compact);

            socket->write(respData);                          // 응답 전송
        }
        // 회원가입 요청일 때
        else if (type == "signup") {
            // 회원가입 데이터 추출
            QString name = obj.value("Name").toString();
            QString id = obj.value("ID").toString();
            QString pw = obj.value("PW").toString();
            QString phone = obj.value("Phone").toString();

            userInfo info;
            info.name = name;
            info.ID = id;
            info.PW = pw;
            info.phoneNum = phone;

            // 회원 정보 파일에 추가
            usermanage.signUp(info);
        }
        // 메세지 전송일 때
        else if (type == "messegesend"){
            // "chatName" 을 가진 방으로 전송
            QString name = obj.value("chatName").toString();
            QString text = obj.value("textMessage").toString();
            QString sendString = "["+name+"] : " + text;

            QJsonObject sendObj;
            sendObj["type"] = "messegesend";
            sendObj["textMessage"] = sendString;
            QJsonDocument sendDoc(sendObj);
            QByteArray sending = sendDoc.toJson(QJsonDocument::Compact);
        }
    }
}

// 연결 해제 처리
void ClientHandler::onDisconnected() {
    qDebug() << "클라이언트 연결 해제";
    socket->deleteLater();
    this->deleteLater();
}
