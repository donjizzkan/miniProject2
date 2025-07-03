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

    ui -> mainStackedWidget -> addWidget(loginView); // index 0 - devwooms
    ui -> mainStackedWidget -> addWidget(signupView); // index 1 - devwooms
    ui -> mainStackedWidget -> addWidget(mainView); // index 2 - devwooms

    // 0번째 먼저 보여주도록 세팅 - devwooms
    ui->mainStackedWidget->setCurrentIndex(0);


    //=========================================
    //              시그널 연결
    //=========================================
    //-----이하 로그인 화면 시그널 처리
    connect(loginView, &LoginView::goToSignup, this, [=]() {                // 로그인 화면에서 goToSignup 시그널 발생할 경우 처리
        ui->mainStackedWidget->setCurrentWidget(signupView);
    });
    /*connect(loginView, &LoginView::goToMain, this, [=]() {                  // 로그인 화면에서 goToMain 시그널 발생할 경우 처리
        ui->mainStackedWidget->setCurrentWidget(mainView);                    // 로그인 기능 구현함에 따라 주석처리
    });*/
    connect(loginView, &LoginView::doSignIn,this,[=](){
        userManage manage;
        QString ID, PW;
        bool val;


        // 입력받은 ID, PW값 가져옴
        ID = loginView->getInsertedID();
        PW = loginView->getInsertedPW();
        val = manage.signIn(ID,PW);
        if (val == true){
            ui->mainStackedWidget->setCurrentWidget(mainView);
        }
        else{
            ui->mainStackedWidget->setCurrentWidget(loginView);
        }
    });


    //-----이하 회원가입 화면 시그널 처리
    connect(signupView, &SignupView::goToLogin, this, [=]() {               // 회원가입 화면에서 goToLogin 시그널 발생할 경우 처리
        ui->mainStackedWidget->setCurrentWidget(loginView);
    });
    connect(signupView, &SignupView::doSignUp, this,[=](){                  // 회원가입 화면에서 doSignUp 시그널 처리
        userManage manage;
        userInfo info;

        info = signupView->getUserInfo();                                   // 입력창에 입력했던 내용 받아오기
        manage.signUp(info);                                                // 입력받은 내용 등록
        ui->mainStackedWidget->setCurrentWidget(loginView);                 // 로그인 화면으로 이동
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
