#include "chattingwindow.h"
#include "ui_chattingwindow.h"

//==================================
//        채팅방 기능 구현
//==================================
ChattingWindow::ChattingWindow(const QString& name, QWidget *parent)
    : QWidget(parent), chatName(name)          // name 변수를 chatName에....
    , ui(new Ui::ChattingWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(chatName);

    QTcpSocket* socket = socketManage::instance().socket();

    //==========================
    //   서버로부터 메세지 받음
    //==========================
    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {        // 소켓 읽게하기
        QByteArray data = socket->readAll();
        //===========================================================================
        // 메세지 전달받는 기능 구현해야대
        //==========================================================================
    });

    //==========================
    //    서버로 메세지 전송
    //==========================
    connect(ui->pushButton_2, &QPushButton::pressed,this,[this,socket](){
        sendingManage sender;
        QString message = ui->lineEdit->text();
        ui->lineEdit->clear();
        sender.sendMessage(chatName,message);
    });
}

ChattingWindow::~ChattingWindow()
{
    delete ui;
}
