#ifndef USERINFOSTORAGE_H
#define USERINFOSTORAGE_H
#include <QString>

// 유저 정보를 userInfoStorage 클래스를 이용해 전역 변수에 저장
// 로그인 할 때 로그인한 유저 이름과 아이디를 받아옴
// 채팅 메세지 전달할 때, 코인 매수/매도할 때 등의 상황에서 getUserInfo 함수를 이용해 정보 받아옴

struct userInfo{
    QString userName;
    QString ID;
};

class userInfoStorage
{
public:
    static userInfoStorage& instance(){
        static userInfoStorage inst;
        return inst;
    }

    void setUserInfo(const QString& name, const QString& id){
        user.userName = name;
        user.ID = id;
    }

    // 유저 정보 받아오기
    userInfo getUserInfo(){
        return user;
    }

private:
    userInfo user;
    userInfoStorage(){}         // 생성자
};

#endif // USERINFOSTORAGE_H
