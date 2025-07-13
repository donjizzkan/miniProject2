#include "chattingwindow.h"
#include "ui_chattingwindow.h"

//==================================
//        채팅방 기능 구현
//==================================
ChattingWindow::ChattingWindow(const QString& name, QWidget *parent)
    : DropWidget(parent), chatViewName(name)          // name 변수를 chatViewName에....
    , ui(new Ui::ChattingWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(chatViewName);

    QTcpSocket* socket = socketManage::instance().socket();

    //==========================
    //   서버로부터 메세지 받음
    //==========================
    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {        // 소켓 읽게하기
        QByteArray data = socket->readAll();

        // error 있거나 전달받지 못했을 때 처리
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if(err.error != QJsonParseError::NoError || !doc.isObject()){
            qDebug() << "메세지 전달 받기 실패";
            return;
        }

        // Json 파일 해부
        QJsonObject obj = doc.object();
        QString type = obj["type"].toString();

        if(type == "messagesend"){
            QString chatName = obj["chatViewName"].toString();
            QString text = obj["textMessage"].toString();

            if(chatName == this->chatViewName){
                ui->textBrowser->append(text);
            }
        }
    });

    //==========================
    //    서버로 메세지 전송
    //==========================
    connect(ui->pushButton_2, &QPushButton::pressed,this,[this,socket](){
        QString message = ui->lineEdit->text();
        ui->lineEdit->clear();
        sendingManage::instance()->sendMessage(chatViewName, message);
    });
}

ChattingWindow::~ChattingWindow()
{
    delete ui;
}
