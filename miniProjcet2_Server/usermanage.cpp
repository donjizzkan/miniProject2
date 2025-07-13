#include "usermanage.h"
#include <QFileInfo>

userManage::userManage(QObject* parent) : QObject(parent){
}
userManage::~userManage(){
}

//=========================================
//                  로그인
//=========================================
bool userManage::signIn(QString& ID, QString& PW, QString& nameOut){
    QFile file("userInfo.json");                        // userInfo 파일 가져오기
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "파일 열기 실패:" << file.errorString();
        return false;
    }
    QByteArray data = file.readAll();                   // 가져온 파일 내용물 다 읽어옴
    file.close();                                       // 파일 닫기
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray userList = doc.array();

    for(const QJsonValue& val : userList){              // 저장되어있던 리스트 훑기
        QJsonObject userObj = val.toObject();

        QString listedID = userObj["ID"].toString();
        QString listedPW = userObj["PW"].toString();
        QString listedName = userObj["name"].toString();
        if(ID==listedID && PW==listedPW){
            qDebug() << "ID : "<<ID<<" 님이 입장하셨습니다.";
            nameOut = listedName;       // 이름 반환
            return true;
        }
    }
    qDebug()<<"ID 또는 PW 불일치";

    return false;
}





//=========================================
//                 회원 가입
//=========================================
void userManage::signUp(userInfo& info){
    // 파일 생성 오류를 대비한 경로 설정
    QFile file("userInfo.json");        // userInfo 파일 가져오기

    // userInfo 파일이 없을 경우 생성
    if(!file.exists()){
        if(file.open(QIODevice::WriteOnly)){
            QJsonArray emptyArray;
            QJsonDocument doc(emptyArray);
            file.write(doc.toJson());
            file.close();
            qDebug()<<"userInfo Json 파일 생성";
        }
        else{
            qWarning()<<"파일 생성 실패";
            return;
        }
    }

    // 전달받은 userInfo를 json Obejct로
    QJsonObject userObj;
    userObj["name"] = info.name;
    userObj["ID"] = info.ID;
    userObj["PW"] = info.PW;
    userObj["phoneNum"] = info.phoneNum;
    userObj["coinCnt"] = info.coinCnt;
    userObj["payment"] = info.payment;
    userObj["money"] = info.money;
    userObj["tradingHis"] = QJsonArray();   // 빈 Array 대입


    // 저장할 내용 이어붙이기 위해 앞내용 읽어옴
    QJsonArray userArray;                   // Json파일내용물 저장될 JsonArray
    if(file.open(QIODevice::ReadOnly)){
        QByteArray readData = file.readAll();
        QJsonDocument dataDoc = QJsonDocument::fromJson(readData);
        userArray = dataDoc.array();
        file.close();
    }
    else{
        qWarning()<<"파일 읽기 실패";
        return;
    }

    userArray.append(userObj);      // json Object를 userArray로

    if(file.open(QIODevice::WriteOnly)){
        QJsonDocument newDoc(userArray);
        file.write(newDoc.toJson());
        file.close();
        qDebug()<<info.name<<" 가입완료";
    }
}
