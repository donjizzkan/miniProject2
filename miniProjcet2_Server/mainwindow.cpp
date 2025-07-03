#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // tcpServer = new QTcpServer(this);
    // connect(tcpServer, SIGNAL(newConnection()), SLOT(clientConnect()));

    // ui -> serverState -> setText(tr("The server is running on port %1.").arg(tcpServer -> serverPort()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
