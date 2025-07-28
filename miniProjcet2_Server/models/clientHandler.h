#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QList>
#include <QString>
#include "usermanage.h"

class ClientHandler : public QObject {
    Q_OBJECT

public:
    explicit ClientHandler(QTcpSocket* socket, QObject* parent = nullptr);

signals:
    void loggedIn(ClientHandler* handler);
    void disconnected(ClientHandler* handler);

public slots:
    void onReadyRead();
    void onDisconnected();
    void sendMessageToClient(QByteArray data);


private:
    QTcpSocket* socket;
    qintptr socketDescriptor;
    QByteArray readBuffer;
    userManage* usermanage;
    
    // 이메일 인증 관련 클라이언트별 변수들
    QString pendingEmail;        // 현재 인증 진행 중인 이메일
    QString savedCode;    // 해당 클라이언트의 인증 코드

};

#endif // CLIENTHANDLER_H
