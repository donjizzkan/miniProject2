#include "clientHandler.h"
#include "servermanager.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>


ClientHandler::ClientHandler(QTcpSocket *socket, QObject *parent)
    : QObject(parent), socket(socket) {
    usermanage = new userManage(this);
    connect(socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);
}

void ClientHandler::onDisconnected() {
    qDebug() << "클라이언트 연결 해제";
    emit disconnected(this);
    socket->deleteLater();
    this->deleteLater();
}

//==========================
//       받은 신호 처리
//==========================
void ClientHandler::onReadyRead() {
    qDebug() << "신호 받음";
    readBuffer.append(socket->readAll());

    while (readBuffer.contains('\n')) {
        int newlineIndex = readBuffer.indexOf('\n');
        QByteArray line = readBuffer.left(newlineIndex);
        readBuffer.remove(0, newlineIndex + 1);

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error != QJsonParseError::NoError) {
            qDebug() << "클라이언트에서 받은 정보 Json 파싱 에러 발생";
            continue;
        }

        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            QString type = obj.value("type").toString();    // 신호의 type 읽어오기

            //==========================
            //       로그인 요청 처리
            //==========================
            if (type == "login") {
                QString ID = obj.value("ID").toString();
                QString PW = obj.value("PW").toString();
                QString nameOut;

                // usermanage로 로그인 시도(성공 시 nameOut에 이름 세팅)
                bool val = usermanage->signIn(ID, PW, nameOut);

                QJsonObject JsonResponse;
                JsonResponse["type"] = "response";
                JsonResponse["result"] = val ? "success" : "fail";
                JsonResponse["name"] = nameOut;

                if (val) {
                    // 1) 로그인 성공 시 유저 정보 전체 내려주기
                    QJsonObject userInfo = usermanage->getUserDetailByName(nameOut);
                    JsonResponse["user"] = userInfo;
                    JsonResponse["history"] = userInfo["tradingHis"];
                }

                // 응답 전송
                QJsonDocument respDoc(JsonResponse);
                QByteArray respData = respDoc.toJson(QJsonDocument::Compact);
                respData.append('\n');
                socket->write(respData);

            //==========================
            //       회원가입 처리
            //==========================
            } else if (type == "signup") {
                QString name = obj.value("Name").toString();
                QString id = obj.value("ID").toString();
                QString pw = obj.value("PW").toString();
                QString phone = obj.value("Phone").toString();

                userInfo info;
                info.name = name;
                info.ID = id;
                info.PW = pw;
                info.phoneNum = phone;

                usermanage->signUp(info);
            //==========================
            //       메세지 전송 처리
            //==========================
            } else if (type == "messagesend") {
                qDebug() << "message 전달받음";
                QString senderName = obj.value("senderName").toString();
                QString chatViewName = obj.value("chatViewName").toString();
                QString text = obj.value("textMessage").toString();
                QString sendString = "[" + senderName + "] : " + text;

                if (usermanage->isBanned(senderName)) {
                    // 금지 상태면 "yourebanned" 신호만 전송
                    QJsonObject resp;
                    resp["type"] = "yourebanned";
                    QJsonDocument doc(resp);
                    socket->write(doc.toJson(QJsonDocument::Compact) + "\n");
                    return;
                }
                QJsonObject sendObj;
                sendObj["type"] = "messagesend";
                sendObj["textMessage"] = sendString;
                sendObj["chatViewName"] = chatViewName;

                QJsonDocument sendDoc(sendObj);
                QByteArray sending = sendDoc.toJson(QJsonDocument::Compact);
                sending.append('\n');

                ServerManager::getInstance().broadcastMessage(sending);
                qDebug() << "클라이언트로 전송 요청 완료";

                // 전송 내용 파일로 저장, DB 폴더에 "채팅방 이름"의 json파일을 생성 및 열람
                QString chatPath = usermanage->getDBPath().replace("userInfo.json", chatViewName + ".json");
                QFile file(chatPath);

                // 파일이 없을 경우 생성
                if (!file.exists()) {
                    if (file.open(QIODevice::WriteOnly)) {
                        QJsonArray emptyArray;
                        QJsonDocument doc(emptyArray);
                        file.write(doc.toJson());
                        file.close();
                        qDebug() << chatViewName <<" Json 파일 생성";
                    } else {
                        qWarning() << "파일 생성 실패";
                        return;
                    }
                }

                // 저장할 내용 이어붙이기 위해 파일의 앞내용 읽어옴
                QJsonArray messageArray;                   // Json파일내용물 저장될 JsonArray
                if(file.open(QIODevice::ReadOnly)){
                    QByteArray readData = file.readAll();
                    QJsonDocument dataDoc = QJsonDocument::fromJson(readData);
                    messageArray = dataDoc.array();
                    file.close();
                }
                else{
                    qWarning()<<"파일 읽기 실패";
                    return;
                }

                messageArray.append(sendString);      // json Object를 userArray로
                if(file.open(QIODevice::WriteOnly)){
                    QJsonDocument newDoc(messageArray);
                    file.write(newDoc.toJson());
                    file.close();
                    qDebug()<<"메세지 저장 완료";
                }

            //==========================
            //       파일 전송 처리
            //==========================
            } else if (type == "filesend") {
                qDebug() << "file 전달받음";
            }
            //==========================
            //      채팅 로그 전송
            //==========================
            else if (type == "givemelog"){
                QString chatViewName = obj.value("chatViewName").toString();
                // 폴더에 있는 파일 읽어옴
                QString chatLogPath = usermanage->getDBPath().replace("userInfo.json", chatViewName + ".json");
                QFile file(chatLogPath);
                QJsonObject JsonResponse;
                JsonResponse["type"] = "messagelog";

                // 파일이 없을 경우 로그 없음 전송
                if (!file.exists()) {
                    JsonResponse["exist"] = "no";   // 존재여부 no
                    QJsonDocument respDoc(JsonResponse);
                    QByteArray respData = respDoc.toJson(QJsonDocument::Compact);
                    respData.append('\n');
                    socket->write(respData);
                } else{     // 파일이 있을 경우 파일 전부 읽어와 내용물 전송
                    file.open(QIODevice::ReadOnly);
                    // 읽어오는데에 쓰는 변수
                    QByteArray readData = file.readAll();
                    QJsonDocument dataDoc = QJsonDocument::fromJson(readData);
                    QJsonArray messageArray = dataDoc.array();
                    JsonResponse["exist"] = "yes";
                    JsonResponse["log"] = messageArray;
                    file.close();
                    // 전송하는데에 쓰는 변수
                    QJsonDocument sendingDoc(JsonResponse);
                    QByteArray sendingData = sendingDoc.toJson(QJsonDocument::Compact);
                    sendingData.append('\n');
                    socket->write(sendingData);
                }
            }
            //==========================
            //     매수/매도 처리
            //==========================
            else if(type == "trade"){
                QString userInfoPath = usermanage->getDBPath();
                QFile file(userInfoPath);
                if(!file.open(QIODevice::ReadOnly)){
                    qDebug()<<"거래 신호 수신, 유저 정보 읽기 실패";
                    return;
                }
                QByteArray data = file.readAll();
                file.close();
                QJsonDocument doc = QJsonDocument::fromJson(data);
                QJsonArray userList = doc.array();

                QString action = obj.value("action").toString();
                QString coin = obj.value("coin").toString();
                double price = obj.value("price").toDouble();
                int amount = obj.value("amount").toInt();
                QString senderName = obj.value("senderName").toString();

                bool updated = false;
                QJsonObject resultObj; // 응답용

                for(int i = 0; i < userList.size(); ++i){
                    QJsonObject userObj = userList[i].toObject();
                    if(userObj["name"].toString() == senderName){
                        QJsonObject coins = userObj["coins"].toObject();
                        double money = userObj["money"].toDouble();
                        double payment = userObj["payment"].toDouble();

                        int currentCoinCnt = coins.value(coin).toInt();

                        // 신규 코인 거래 대응
                        if(!coins.contains(coin))
                            coins[coin] = 0;

                        if(action == "buy"){
                            double totalCost = price * amount;
                            if(money >= totalCost){
                                money -= totalCost;
                                payment += totalCost;
                                coins[coin] = currentCoinCnt + amount;
                                userObj["money"] = money;
                                userObj["coins"] = coins;
                                userObj["payment"] = payment;
                                updated = true;
                                qDebug()<<"매수 성공";
                            } else {
                                qDebug()<<"매수 실패: 잔액 부족";
                            }
                        }
                        else if(action == "sell"){
                            if(currentCoinCnt >= amount){
                                money += price * amount;
                                payment -= price * amount;
                                coins[coin] = currentCoinCnt - amount;
                                userObj["money"] = money;
                                userObj["coins"] = coins;
                                userObj["payment"] = payment;
                                updated = true;
                                qDebug()<<"매도 성공";
                            } else {
                                qDebug()<<"매도 실패: 코인 부족";
                            }
                        }

                        // 거래내역 기록
                        QJsonArray tradingHis = userObj["tradingHis"].toArray();
                        QJsonObject record;
                        record["action"] = action;
                        record["coin"] = coin;
                        record["price"] = price;
                        record["amount"] = amount;
                        record["datetime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
                        tradingHis.append(record);
                        userObj["tradingHis"] = tradingHis;

                        userList[i] = userObj;
                        resultObj = userObj; // 응답으로 현재 유저정보를 그대로 내려주고 싶다면
                        break;
                    }
                }

                // 변경 사항 저장
                if(updated){
                    if(file.open(QIODevice::WriteOnly)){
                        QJsonDocument newDoc(userList);
                        file.write(newDoc.toJson());
                        file.close();
                    }
                }

                // 거래 응답 전송
                QJsonObject resp;
                resp["type"] = "traderesponse";
                resp["result"] = updated ? "success" : "fail";
                resp["action"] = action;
                resp["coin"] = coin;
                resp["amount"] = amount;
                resp["history"] = resultObj["tradingHis"];

                // [수정된 부분] user 객체 통째로 넘기는 대신 핵심 정보만 직접 넘김
                resp["money"] = resultObj["money"];
                resp["payment"] = resultObj["payment"];
                resp["coins"] = resultObj["coins"];
                // resp["user"] = resultObj; // 이 라인은 이제 필요 없어!

                QJsonDocument respDoc(resp);
                QByteArray respData = respDoc.toJson(QJsonDocument::Compact);
                respData.append('\n');
                socket->write(respData);
                qDebug() << "거래 응답 전송";
            }
            //==========================
            //       신고 처리
            //==========================
            else if (type == "report") {
                QString targetName = obj.value("targetName").toString();
                QString reason = obj.value("reason").toString();

                // 신고 처리 (userManage에 함수 분리 추천)
                usermanage->increaseReport(targetName, reason);

                // 필요시 신고 누적 결과 응답
                bool isBanned = usermanage->isBanned(targetName);
                QJsonObject resp;
                resp["type"] = "reportresult";
                resp["result"] = "ok";
                resp["targetBanned"] = isBanned;
                socket->write(QJsonDocument(resp).toJson(QJsonDocument::Compact) + "\n");
            }

        }
    }
}

void ClientHandler::sendMessageToClient(QByteArray data) {
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(data);
        socket->flush();
    }
}

