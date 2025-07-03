#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QNetworkInterface>
#include <QHostAddress>


#define PORT_NUM  51234


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize client socket list
    clientSocketList = new QList<QTcpSocket*>();

    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), SLOT(clientConnect()));

    // Set specific port (you can change this to any port you want)
    quint16 port = PORT_NUM;
    
    // Listen on all network interfaces (QHostAddress::Any allows external connections)
    if(!tcpServer->listen(QHostAddress::Any, port)){
        QMessageBox::critical(this, tr("Echo Server"), tr("Unable to start the server.%1.").arg(tcpServer -> errorString()));
        close();
        return;
    }

    // Get external IP address
    QString externalIP = getExternalIP();
    QString serverInfo = QString("%1:%2").arg(externalIP).arg(tcpServer->serverPort());
    
    ui->serverState->setText(tr("The server is running on %1").arg(serverInfo));
    qDebug() << "Server started on:" << serverInfo;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getExternalIP()
{
    // Get all network interfaces
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    
    for (const QNetworkInterface &interface : std::as_const(interfaces)) {
        // Skip loopback and non-active interfaces
        if (interface.flags() & QNetworkInterface::IsLoopBack || 
            !(interface.flags() & QNetworkInterface::IsUp) ||
            !(interface.flags() & QNetworkInterface::IsRunning)) {
            continue;
        }
        
        // Get all IP addresses for this interface
        QList<QNetworkAddressEntry> addressEntries = interface.addressEntries();
        for (const QNetworkAddressEntry &entry : addressEntries) {
            QHostAddress address = entry.ip();
            // Look for IPv4 addresses that are not localhost
            if (address.protocol() == QAbstractSocket::IPv4Protocol &&
                address != QHostAddress::LocalHost) {
                return address.toString();
            }
        }
    }
    
    // If no external IP found, return localhost
    return "127.0.0.1";
}

void MainWindow::clientConnect()
{
    // Get the pending connection
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    
    // Add to client socket list
    clientSocketList->append(clientSocket);
    
    // Optional: Connect to disconnected signal to remove from list when client disconnects
    connect(clientSocket, &QTcpSocket::disconnected, [this, clientSocket](){
        clientSocketList->removeOne(clientSocket);
        clientSocket->deleteLater();
    });
    
    // Print connection info
    qDebug() << "New client connected. Total clients:" << clientSocketList->size();
}

