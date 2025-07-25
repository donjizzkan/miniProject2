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
};

#endif // SENDINGMANAGE_H
