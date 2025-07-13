#ifndef SENDINGMANAGE_H
#define SENDINGMANAGE_H

#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>    // Json관련 라이브러리
#include <QJsonArray>       // Json관련 라이브러리
#include <QJsonObject>      // Json관련 라이브러리
#include "socketManage.h"

class sendingManage : public QObject {
    Q_OBJECT
public:
    // 로그인 화면 정보 서버에 전달
    void sendLogIn(QString& ID, QString& PW);

    // 회원가입 화면 정보 서버에 전달
    void sendSignUp(QString& Name, QString& ID, QString& PW, QString& PhoneNum);

    // 채팅창이름과 메세지내용을 전달
    void sendMessage(QString& chatViewName, QString& textMessage);

    // 전송자 이름(senderName 변수 setter)
    void setSenderName(QString& name) {senderName = name;}


private:
    QString senderName;
};

#endif // SENDINGMANAGE_H
