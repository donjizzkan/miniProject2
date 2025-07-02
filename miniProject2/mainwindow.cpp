#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginview.h"
#include "signupview.h"
#include "mainview.h"
#include "userManage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LoginView *loginView = new LoginView;
    SignupView *signupView = new SignupView;
    MainView *mainView = new MainView;

    loginView->setStyleSheet("background-color: lightblue;");
    signupView->setStyleSheet("background-color: lightgreen;");
    mainView->setStyleSheet("background-color: #afaf;");

    // ui->mainStackedWidget->setStyleSheet("background-color: lightgray;");

    ui -> mainStackedWidget -> addWidget(loginView); // index 0
    ui -> mainStackedWidget -> addWidget(signupView); // index 1
    ui -> mainStackedWidget -> addWidget(mainView); // index 1

    // 0번째 먼저 보여주도록 세팅
    ui->mainStackedWidget->setCurrentIndex(0);


    //=========================================
    //              시그널 연결
    //=========================================
    connect(loginView, &LoginView::goToSignup, this, [=]() {                // 로그인 화면에서 goToSignup 시그널 발생할 경우 처리
        ui->mainStackedWidget->setCurrentWidget(signupView);
    });
    connect(loginView, &LoginView::goToMain, this, [=]() {                  // 로그인 화면에서 goToMain 시그널 발생할 경우 처리
        ui->mainStackedWidget->setCurrentWidget(mainView);
    });
    connect(signupView, &SignupView::goToLogin, this, [=]() {               // 회원가입 화면에서 goToLogin 시그널 발생할 경우 처리
        ui->mainStackedWidget->setCurrentWidget(loginView);
    });
    connect(signupView, &SignupView::doSignUp, this,[=](){                  // 회원가입 화면에서 doSignUp 시그널 처리
        userManage manage;
        manage.signUp();            // signUp 실행
        ui->mainStackedWidget->setCurrentWidget(loginView);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
