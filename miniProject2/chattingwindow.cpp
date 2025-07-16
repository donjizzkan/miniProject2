#include "chattingwindow.h"
#include "ui_chattingwindow.h"
#include "socketManage.h"    // socketManage 사용을 위해 추가
#include "sendingManage.h"   // sendingManage 사용을 위해 추가
#include <QDebug>            // 디버그 출력을 위해 추가

#include <QFileDialog>

//==================================
//        채팅방 기능 구현
//==================================
ChattingWindow::ChattingWindow(const QString& name, QWidget *parent)
    : DropWidget(parent), chatViewName(name)          // name 변수를 chatViewName에....
    , ui(new Ui::ChattingWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(chatViewName);

    //==========================
    //   서버로부터 메세지 받음
    //==========================
    // SocketManage에서 방출하는 chatMessageReceived 시그널을 받도록 변경
    connect(&SocketManage::instance(), &SocketManage::chatMessageReceived, this, [this](const QJsonObject& message) {
        qDebug() << "ChattingWindow: chatMessageReceived 시그널 받음";
        // 이 message는 이미 SocketManage에서 파싱된 JSON 객체임
        // textBrowser에 message 띄우기
        if(message["type"].toString() == "messagesend"){ // 타입이 'messagesend'인지 다시 확인 (안전성)
            QString chatName = message["chatViewName"].toString();
            QString text = message["textMessage"].toString();
            if(chatName == this->chatViewName){ // 현재 채팅방 이름과 일치하는 메시지만 표시
                ui->textBrowser->append(text);
                qDebug() << chatName<<"으로 메세지 띄움";
            }
        }
    });

    //==========================
    //    서버로 메세지 전송
    //==========================
    connect(ui->pushButton_2, &QPushButton::pressed,this,[this](){
        QString message = ui->lineEdit->text();
        if (!message.isEmpty()) { // 빈 메시지는 전송하지 않음
            ui->lineEdit->clear();
            sendingManage::instance()->sendMessage(chatViewName, message);
            qDebug() << "메시지 전송 시도: " << message;
        }
    });

    //==========================
    //   버튼 클릭시 파일 - devwooms
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
