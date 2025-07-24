#ifndef SIGNUPVIEW_H
#define SIGNUPVIEW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "models/userManage.h"

class SignupView : public QWidget
{
    Q_OBJECT

public:
    explicit SignupView(QWidget *parent = nullptr);
    ~SignupView();

    // line Edit에 입력된 정보 받아오기
    userInfo getUserInfo();


private:
    QLineEdit *name_LineEdit;
    QLineEdit *id_LineEdit;
    QLineEdit *pw_LineEdit;
    QLineEdit *phoneNUM_LineEdit;
    QLineEdit *email_LineEdit;
    QLineEdit *emailCheck_LineEdit;
    QLabel *label_2;
    QPushButton *emailCheck;
    QLabel *emailCheck_Lable;
    QPushButton *emailCheckNUM;
    QPushButton *signup_Button;
    QPushButton *cancel_Button;

    // test
    QString savedCode;
    void sendEmail(QString email, QString code);
    
    void setupUI();
    // 초기화 및 설정 메서드들
    void setupInputFields();
    void connectSignals();
    void checkEmail();
    void checkEmailNum();

signals:
    void goToLogin();
    void doSignUp();            // signUp 함수 호출 시그널

};

#endif // SIGNUPVIEW_H
