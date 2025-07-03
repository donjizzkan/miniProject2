#include "loginview.h"
#include "ui_loginview.h"
#include "socketManage.h"

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    //==========================
    //        소켓 연결
    //==========================

    ui->setupUi(this);
    // socket Manage에서 생성된 socket 객체 가져오기
    QTcpSocket* socket = socketManage::instance().socket();
    // 호스트에 연결
    socket->connectToHost("192.168.137.1",51234);
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
