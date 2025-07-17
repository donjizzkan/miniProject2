#ifndef USERMANAGE_H
#define USERMANAGE_H

#include <QMainWindow>
#include <QDateTime>        // 날짜 및 시간 관련 라이브러리
#include <QFile>            // 파일 관련 라이브러리
#include <QJsonDocument>    // Json관련 라이브러리
#include <QJsonArray>       // Json관련 라이브러리
#include <QJsonObject>      // Json관련 라이브러리
#include <QDebug>           // 디버깅 출력 라이브러리

using namespace std;




// 유저 정보 저장
class userInfo {
public:

    //========== 개인정보
    QString name;           // 이름
    QString ID;             // 아이디
    QString PW;             // 비밀번호
    QString phoneNum;           // 전화번호


    //========== 보유 자산
    int coinCnt;              // 코인 개수
    int payment;              // 매입 금액 (매수하는데에 쓴 총액)
    int money;                // 보유 현금


    //========== 거래 관련 정보
    struct tradeInfo{
        QDateTime tradeDate;      // 거래 일시
        int tradeCnt;             // 거래 코인수량
        int tradePrice;           // 거래당시 코인금액
    };

    QList<tradeInfo> tradingHis;    // 거래시마다 위 구조체 저장할 QList
};


//=====userManage 클래스는 서버 프로젝트로 이전되었습니다.

#endif // USERMANAGE_H
