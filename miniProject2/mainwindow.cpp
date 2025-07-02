#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "loginview.h"
#include "signupview.h"
#include "mainview.h"

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

    ui -> mainStackedWidget -> addWidget(loginView); // index 0
    ui -> mainStackedWidget -> addWidget(signupView); // index 1
    ui -> mainStackedWidget -> addWidget(mainView); // index 1

    // 0번째 먼저 보여주도록 세팅
    ui->mainStackedWidget->setCurrentIndex(0);

    // 시그널 연결
    connect(loginView, &LoginView::goToSignup, this, [=]() {
        ui->mainStackedWidget->setCurrentWidget(signupView);
    });
    connect(loginView, &LoginView::goToMain, this, [=]() {
        ui->mainStackedWidget->setCurrentWidget(mainView);
    });


    connect(signupView, &SignupView::goToLogin, this, [=]() {
        ui->mainStackedWidget->setCurrentWidget(loginView);
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}
