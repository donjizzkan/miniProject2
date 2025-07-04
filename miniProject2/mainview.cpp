#include "mainview.h"
#include "ui_mainview.h"

#include "chartstoolbox.h"
#include "chattinglistmanager.h"

MainView::MainView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainView)
{
    ui->setupUi(this);


    // ==============================
    // 거래 탭 - devwooms
    // ==============================

    // ChartsToolBox 인스턴스 생성 - devwooms
    ChartsToolBox *chartBox = new ChartsToolBox(this);
    chartBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // tab의 layout에 추가 - devwooms
    ui->chartTab->addWidget(chartBox);


    // ==============================
    // 오른쪽 채팅 ToolBox - devwooms
    // ==============================

    ChattingListManager *chattingListManager = new ChattingListManager;

    chattingListManager->settingChattingToolBox(ui->chatting_ToolBox);

    chattingListManager->settingConnectListWidget(ui->connect_listWidget);
    chattingListManager->settingOneByOneListWidget(ui->oneByone_listWidget);
    chattingListManager->settingOneByMoreListWidget(ui->oneByMore_listWidget);

}

MainView::~MainView()
{
    delete ui;
}
