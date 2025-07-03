#include "chattingwindow.h"
#include "ui_chattingwindow.h"

ChattingWindow::ChattingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChattingWindow)
{
    ui->setupUi(this);
}

ChattingWindow::~ChattingWindow()
{
    delete ui;
}
