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
    // 이름 싱글턴으로 선언해야 모든 cpp파일에서 접근 가능하므로 싱글턴 선언
    static sendingManage* instance();
    // 전송자 이름(senderName 변수 setter)
    void setSenderName(const QString& name);


    // 로그인 화면 정보 서버에 전달
    void sendLogIn(QString& ID, QString& PW);

    // 회원가입 화면 정보 서버에 전달
    void sendSignUp(QString& Name, QString& ID, QString& PW, QString& PhoneNum);

    // 채팅창이름과 메세지내용을 전달
    void sendMessage(QString& chatViewName, QString& textMessage);

    // 파일 전달
    void sendFile(QStringList filePaths, QString& chatViewName);

    // 로그를 달라는 신호 전달
    void giveMeLog(QString& chatViewName);

    static sendingManage* m_instance;
    sendingManage();

private:
    QString senderName;
};

#endif // SENDINGMANAGE_H
