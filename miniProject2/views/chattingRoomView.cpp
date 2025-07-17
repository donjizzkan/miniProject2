#include "chattingRoomView.h"
#include "ui_chattingRoomView.h"
#include "models/socketManage.h"    // socketManage 사용을 위해 추가
#include "models/sendingManage.h"   // sendingManage 사용을 위해 추가
#include <QDebug>            // 디버그 출력을 위해 추가

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

//==================================
//        채팅방 기능 구현
//==================================
ChattingRoomView::ChattingRoomView(const QString& name, QWidget *parent)
    : DropWidget(parent, name), chatViewName(name)          // name 변수를 chatViewName에....
    , ui(new Ui::ChattingRoomView)
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
    //   서버로부터 파일 받음
    //==========================
    connect(&SocketManage::instance(), &SocketManage::fileReceived, this, [this](const QJsonObject& fileData) {
        qDebug() << "ChattingWindow: fileReceived 시그널 받음";
        
        if(fileData["type"].toString() == "filesend") {
            QString chatName = fileData["chatViewName"].toString();
            if(chatName == this->chatViewName) { // 현재 채팅방과 일치하는 파일만 처리
                QString fileName = fileData["fileName"].toString();
                int fileSize = fileData["fileSize"].toInt();
                
                // 채팅창에 파일 수신 메시지 표시
                ui->textBrowser->append(QString("[파일 수신] %1 (%2 bytes)").arg(fileName).arg(fileSize));
                qDebug() << "파일 수신:" << fileName;
                
                // 여기에 파일 저장 로직을 추가할 수 있음
                // QString base64Data = fileData["fileData"].toString();
                // QByteArray fileContent = QByteArray::fromBase64(base64Data.toUtf8());
                // 파일 저장 처리...
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
            
            // 파일 전송 확인
            QMessageBox::StandardButton reply = QMessageBox::question(
                this,
                "파일 전송",
                QString("선택된 파일을 전송하시겠습니까?\n%1").arg(QFileInfo(filePath).fileName()),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No
                );
            
            if (reply == QMessageBox::Yes) {
                QStringList filePaths;
                filePaths << filePath;
                sendingManage::instance()->sendFile(filePaths, chatViewName);
                qDebug() << "파일 전송 요청 완료";
            } else {
                qDebug() << "파일 전송 취소";
            }
        }
    });
    //==========================
    //   드래그 앤 드랍시 파일 - devwooms
    //==========================
    connect(this, &DropWidget::fileDropped, this, [this](const QStringList& filePaths) {
        qDebug() << "파일 드롭됨:" << filePaths;
        // sendingManage를 통해 파일 전송
        sendingManage::instance()->sendFile(filePaths, chatViewName);
        qDebug() << "파일 전송 요청 완료";
    });
}

ChattingRoomView::~ChattingRoomView()
{
    delete ui;
}
