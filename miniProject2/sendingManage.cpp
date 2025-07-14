#include "sendingManage.h"

// 무언가 전달할 때 무조건 메세지 type 포함해서!!!
// 메세지 type의 종류(모두 소문자로 작성할것)
// login, signup, response(success,fail만 result에 대입), messagesend

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

    QTcpSocket* socket = SocketManage::instance().socket();

    qDebug() << "현재 소켓 : " << socket;
    socket->write(sendingArray);
    qDebug()<< chatViewName << "으로" << senderName << "의" << textMessage <<" 전송";
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




