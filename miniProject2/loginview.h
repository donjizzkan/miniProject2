#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include <QJsonDocument>    // Json관련 라이브러리
#include <QJsonArray>       // Json관련 라이브러리
#include <QJsonObject>      // Json관련 라이브러리
#include <QMessageBox>
#include "mainwindow.h"
namespace Ui {
class LoginView;
}

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

    // 입력받은 아이디와 패스워드 가져오는 함수
    QString getInsertedID();
    QString getInsertedPW();


private:
    Ui::LoginView *ui;

// 메인윈도우에게 알릴 신호
signals:
    void goToSignup();
    void goToMain();
    void doSignIn();            // signIn 함수 호출 시그널
};

#endif // LOGINVIEW_H
