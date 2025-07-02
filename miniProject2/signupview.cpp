#include "signupview.h"
#include "ui_signupview.h"
#include "userManage.h"     // 회원가입 및 로그인시 사용

SignupView::SignupView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignupView)
{
    ui->setupUi(this);
    userManage* manage = new userManage(this);

    connect(ui->cancel_Button, &QPushButton::clicked, this, &SignupView::goToLogin);
    connect(ui->signup_Button, &QPushButton::clicked, manage, &userManage::doSignUp);
}

SignupView::~SignupView()
{
    delete ui;
}
