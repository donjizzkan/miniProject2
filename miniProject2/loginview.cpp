#include "loginview.h"
#include "ui_loginview.h"
#include "socketManage.h"
#include <QtNetwork>

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);
    // 소켓 Manage에서 생성된 socket 객체 가져오기
    QTcpSocket* socket = socketManage::instance().socket();
    socket->connectToHost("192.168.2.235",51234);
    connect(socket, &QTcpSocket::connected, this, [](){
        qDebug("서버 연결 완료");
    });
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
