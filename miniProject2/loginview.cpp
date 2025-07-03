#include "loginview.h"
#include "ui_loginview.h"

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);

    connect(ui->signup_Button, &QPushButton::clicked, this, &LoginView::goToSignup);
    connect(ui->login_Button, &QPushButton::clicked, this, &LoginView::doSignIn);

}

LoginView::~LoginView()
{
    delete ui;
}

QString LoginView::getInsertedID(){
    QString getID;
    getID = ui->id_LineEdit->text();
    return getID;
}
QString LoginView::getInsertedPW(){
    QString getPW;
    getPW = ui->pw_LineEdit->text();
    return getPW;
}
