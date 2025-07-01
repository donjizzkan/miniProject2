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
    SingupView *singupView = new SingupView;
    MainView *mainView = new MainView;

    loginView->setStyleSheet("background-color: lightblue;");
    singupView->setStyleSheet("background-color: lightgreen;");
    mainView->setStyleSheet("background-color: #afaf;");

    // ui->mainStackedWidget->setStyleSheet("background-color: lightgray;");

    ui -> mainStackedWidget -> addWidget(loginView); // index 0
    ui -> mainStackedWidget -> addWidget(singupView); // index 1
    ui -> mainStackedWidget -> addWidget(mainView); // index 1

    // 0번째 먼저 보여주도록 세팅
    ui->mainStackedWidget->setCurrentIndex(0);

    // 시그널 연결
    connect(loginView, &LoginView::goToSignup, this, [=]() {
        ui->mainStackedWidget->setCurrentWidget(singupView);
    });
    connect(loginView, &LoginView::goToMain, this, [=]() {
        ui->mainStackedWidget->setCurrentWidget(mainView);
    });


    connect(singupView, &SingupView::goToLogin, this, [=]() {
        ui->mainStackedWidget->setCurrentWidget(loginView);
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}
