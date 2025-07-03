#include "sendingManage.h"

// 무언가 전달할 때 무조건 메세지 타입 포함해서!!!

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

    QTcpSocket* socket = socketManage::instance().socket();
    socket->write(sendingArray);
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

    QTcpSocket* socket = socketManage::instance().socket();
    socket->write(sendingArray);
    qDebug() << "회원가입용 데이터:" << sendingArray;
    qDebug() << "회원가입용 소켓 상태:" << socket->state();

}
