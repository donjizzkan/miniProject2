#include "mainview.h"
#include "ui_mainview.h"

#include "chartstoolbox.h"

MainView::MainView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainView)
{
    ui->setupUi(this);


    // ==============================
    // 거래 탭
    // ==============================

    // ChartsToolBox 인스턴스 생성
    ChartsToolBox *chartBox = new ChartsToolBox(this);
    chartBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // tab의 layout에 추가
    ui->chartTab->addWidget(chartBox);

}

MainView::~MainView()
{
    delete ui;
}
