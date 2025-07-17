#ifndef CLIENTSETUP_H
#define CLIENTSETUP_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QList>

class ClientSetup : public QObject {
    Q_OBJECT
public:
    explicit ClientSetup(qintptr socketDescriptor, QList<ClientSetup*>* handlerList, QObject* parent = nullptr);

public slots:
    void start();
private:
    QTcpSocket* socket;
    qintptr socketDescriptor;
    QList<ClientSetup*>* clientList;
signals:
    void socketReady(QTcpSocket* socket);
};


#endif // CLIENTSETUP_H
