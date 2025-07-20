#include "sendingManage.h"
#include "filesendmanager.h"

// 무언가 전달할 때 무조건 메세지 type 포함해서!!!
// 메세지 type의 종류(모두 소문자로 작성할것) :
//      login, signup, response(success,fail만 result에 대입), messagesend, givemelog, messagelog

//==========================
//       ID, PW 전달
//==========================
void sendingManage::sendLogIn(QString& ID, QString& PW){
    // Json 형태로 전달
    QJsonObject sendingObj;
    sendingObj["type"] = "login";
    sendingObj["ID"] = ID;
    sendingObj["PW"] = PW;
    QJsonDocument doc(sendingObj);
    QByteArray sendingArray(doc.toJson(QJsonDocument::Compact));
    sendingArray.append('\n');
    QTcpSocket* socket = SocketManage::instance().socket();
    qDebug() << "현재 소켓 : " << socket;
    socket->write(sendingArray);
    socket->flush();
    qDebug() << "서버로 로그인 요청";
}


//==========================
//       회원가입 요청
//==========================
void sendingManage::sendSignUp(QString& Name, QString& ID, QString& PW, QString& PhoneNum){
    QJsonObject sendingObj;
    sendingObj["type"] = "signup";
    sendingObj["Name"] = Name;
    sendingObj["ID"] = ID;
    sendingObj["PW"] = PW;
    sendingObj["Phone"] = PhoneNum;
    QJsonDocument doc(sendingObj);
    QByteArray sendingArray(doc.toJson(QJsonDocument::Compact));
    sendingArray.append('\n');
    QTcpSocket* socket = SocketManage::instance().socket();
    socket->write(sendingArray);
    qDebug() << "현재 소켓 : " << socket;
    qDebug() << "서버로 회원가입 요청";
}

//==========================
//      채팅메세지 전송
//==========================
void sendingManage::sendMessage(QString& chatViewName, QString& textMessage){
    QJsonObject sendingObj;
    sendingObj["type"] = "messagesend";
    sendingObj["chatViewName"] = chatViewName;
    sendingObj["senderName"] = senderName;
    sendingObj["textMessage"] = textMessage;
    QJsonDocument doc(sendingObj);
    QByteArray sendingArray(doc.toJson(QJsonDocument::Compact));
    sendingArray.append('\n');
    QTcpSocket* socket = SocketManage::instance().socket();

    qDebug() << "현재 소켓 : " << socket;
    socket->write(sendingArray);
    qDebug()<< chatViewName << "으로" << senderName << "의" << textMessage <<" 전송";
}

//==========================
//      파일 전송
//==========================
void sendingManage::sendFile(QStringList filePaths, QString& chatViewName){
    qDebug() << "sendingManage.cpp sendFile";
    QTcpSocket* socket = SocketManage::instance().socket();
    FileSendManager *fileSendManager = new FileSendManager();
    fileSendManager->sendFile(socket, "filesend", chatViewName, filePaths);
}

//==========================
//  채팅 로그 달라는 신호 전달
//==========================
void sendingManage::giveMeLog(QString& chatViewName){
    qDebug()<<"서버로 채팅 로그 전송 요청";
    QTcpSocket* socket = SocketManage::instance().socket();
    QJsonObject sendingObj;
    sendingObj["type"] = "givemelog";
    sendingObj["chatViewName"] = chatViewName;
    QJsonDocument doc(sendingObj);
    QByteArray sendingArray(doc.toJson(QJsonDocument::Compact));
    sendingArray.append('\n');

    socket->write(sendingArray);
}

sendingManage* sendingManage::m_instance = nullptr;
sendingManage::sendingManage(){

}
sendingManage* sendingManage::instance(){
    if(!m_instance){
        m_instance = new sendingManage;
        return m_instance;
    }
    return m_instance;
}

void sendingManage::setSenderName(QString& name) {
    senderName = name;
}




