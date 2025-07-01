#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "loginview.h"
#include "singupview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LoginView *loginPage = new LoginView;
    SingupView *singupPage = new SingupView;

    loginPage->setStyleSheet("background-color: lightblue;");
    singupPage->setStyleSheet("background-color: lightgreen;");

    ui->mainStackedWidget->setStyleSheet("background-color: lightgray;");

    ui -> mainStackedWidget -> addWidget(loginPage); // index 0
    ui -> mainStackedWidget -> addWidget(singupPage); // index 1

    // 0번째 먼저 보여주도록 세팅
    ui->mainStackedWidget->setCurrentIndex(0);


    // 시그널 연결
    connect(loginPage, &LoginView::goToSignup, this, [=]() {
        ui->mainStackedWidget->setCurrentWidget(singupPage);
    });
    connect(singupPage, &SingupView::goToLogin, this, [=]() {
        ui->mainStackedWidget->setCurrentWidget(loginPage);
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}
