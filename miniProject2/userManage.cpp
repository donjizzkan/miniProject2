#include "userManage.h"

userManage::userManage(QObject* parent) : QObject(parent){

}
userManage::~userManage(){

}
//=========================================
//                  로그인
//=========================================
void userManage::signIn(){
    QFile file("userInfo.json");        // 유저 정보 파일 열기
}

//=========================================
//                 회원 가입
//=========================================
void userManage::signUp(){
    QFile file("userInfo.json");        // userInfo 파일 열기

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
        }
    }

    if(file.open(QIODevice::WriteOnly)){

    }

}
