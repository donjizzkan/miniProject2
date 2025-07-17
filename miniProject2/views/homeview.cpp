#include "homeview.h"
#include "linechartview.h"
#include "ui_homeview.h"

#include "chartstoolbox.h"
#include "models/chattinglistmanager.h"

HomeView::HomeView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeView)
{
    ui->setupUi(this);


    // ==============================
    // 거래 탭 - devwooms
    // ==============================
    QComboBox *combo = ui->comboBox;

    // ChartsToolBox 인스턴스 생성 - devwooms
    chartBox = new ChartsToolBox(this); // 멤버 변수로 저장 - devwooms
    chartBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ui->comboBox->addItem(QIcon(":/assets/assets/Bitcoin.png"), "KRW-BTC / 비트코인 / Bitcoin");
    ui->comboBox->addItem(QIcon(":/assets/assets/Ethereum.jpg"), "KRW-ETH / 이더리움 / Ethereum");
    ui->comboBox->addItem(QIcon(":/assets/assets/XRP.png"), "KRW-XRP / 엑스알피 [리플] / XRP");
    ui->comboBox->addItem(QIcon(":/assets/assets/Dogecoin.svg"), "KRW-DOGE / 도지코인 / Dogecoin");
    ui->comboBox->addItem(QIcon(":/assets/assets/PEPE.jpg"), "KRW-PEPE / 페페 / PEPE");

    // comboBox에서 선택시 선택 코인의 데이터를 가져오도록 연결
    connect(combo, &QComboBox::currentIndexChanged, this, [this, combo](int index) {
        QString text = combo->itemText(index);
        qDebug() << "선택된 항목:" << text;
        
        // 코인 심볼 추출 (예: "KRW-BTC / 비트코인 / Bitcoin" → "krw-btc") - devwooms
        QString coinSymbol = text.split(" / ").first().toLower();
        qDebug() << "추출된 코인 심볼:" << coinSymbol;
        
        // 코인 설정 변경 - devwooms
        if (chartBox && chartBox->getLineChart()) {
            chartBox->getLineChart()->setCoin(coinSymbol);
            chartBox->getLineChart()->lineDataManager->update();
        }
        if (chartBox && chartBox->getCandleChart()) {
            chartBox->getCandleChart()->setCoin(coinSymbol);
            chartBox->getCandleChart()->candleDataManager->update();
        }
    });

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

HomeView::~HomeView()
{
    delete ui;
}
