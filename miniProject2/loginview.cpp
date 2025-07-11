#include "loginview.h"
#include "qjsonobject.h"
#include "ui_loginview.h"
#include "socketManage.h"

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{

    ui->setupUi(this);

    //==========================
    //        소켓 연결
    //==========================

    // socket Manage에서 생성된 socket 객체 가져오기
    QTcpSocket* socket = socketManage::instance().socket();

    // 소켓 연결
    // 192.168.2.235 => devwooms
    // 192.168.137.1 => donjizzkan
    socket->connectToHost("192.168.137.1",51234);
    connect(socket, &QTcpSocket::connected, this, [](){
        qDebug("서버 연결 완료");
    });

    //===========================
    // 통과 신호 받으면 메인화면으로
    //===========================
    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {        // 소켓 읽게하기
        QByteArray data = socket->readAll();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject obj = doc.object();
            if (obj.value("type").toString() == "response") {
                QString result = obj.value("result").toString();
                // 로그인 성공/실패에 따라 행동 분기
                if (result == "success") {
                    QString nickName = obj.value()
                    emit goToMain();               // 로그인 화면으로 이동
                } else {
                    QMessageBox::warning(this, "로그인 실패", "ID 또는 PW가 틀렸습니다!");
                }
            }
        }
    });

    connect(ui->pw_LineEdit, &QLineEdit::returnPressed, this, [=](){
        ui->login_Button->click();
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
