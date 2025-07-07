#include "chattingwindow.h"
#include "ui_chattingwindow.h"

#include <QFileDialog>

//==================================
//        채팅방 기능 구현
//==================================
ChattingWindow::ChattingWindow(const QString& name, QWidget *parent)
    : DropWidget(parent), chatName(name)          // name 변수를 chatName에....
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

    //==========================
    //   버튼 클릭시 파일
    //==========================
    connect(ui->pushButton, &QPushButton::pressed, this, [this](){
        QString filePath = QFileDialog::getOpenFileName(this, "파일 선택", "", "모든 파일 (*)");
        if (!filePath.isEmpty()) {
            qDebug() << "선택된 파일:" << filePath;
            // 파일 처리 코드...
        }
    });
}

ChattingWindow::~ChattingWindow()
{
    delete ui;
}
