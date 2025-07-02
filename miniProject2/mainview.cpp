#include "mainview.h"
#include "ui_mainview.h"

MainView::MainView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainView)
{
    ui->setupUi(this);

    setStyleSheet("background-color: lightblue;");
}

MainView::~MainView()
{
    delete ui;
}
