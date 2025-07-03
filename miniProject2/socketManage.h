#ifndef SOCKETMANAGE_H
#define SOCKETMANAGE_H
#include <QTcpSocket>
#include <QDebug>

class socketManage : public QObject {
    Q_OBJECT
public:
    static socketManage& instance(){
        static socketManage inst;
        return inst;
    }
    socketManage() {}
    QTcpSocket m_socket;
    QTcpSocket* socket(){return &m_socket;}                  // 소켓
    void connectToSever(const QString& host, quint16 port);
};

#endif // SOCKETMANAGE_H
