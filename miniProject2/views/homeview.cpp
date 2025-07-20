#include "homeview.h"
#include "linechartview.h"
#include "ui_homeview.h"
#include "models/sendingManage.h"
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

    //==========================
    //  채팅 로그 달라는 신호 전달
    //==========================
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        QString action;
        int amount = 0;     // 수량
        // 매수버튼(radioButton) 선택되어있을 경우
        if (ui->radioButton->isChecked()) {
            action = "buy";
            amount = ui->spinBox->value();
        // 매도버튼(radioButton_2) 선택되어있을 경우
        } else if (ui->radioButton_2->isChecked()) {
            action = "sell";
            amount = ui->spinBox_2->value();
        }
        // 현재 열람중인 종목 이름 가져오기
        QString comboText = ui->comboBox->itemText(ui->comboBox->currentIndex());
        // 첫 단어만 가져오게 예(krw-btc)
        QString coinName = comboText.split(" / ").first().toLower();
        double price = chartBox->getLineChart()->getLatestPrice();  // 현재 가격

        sendingManage sending;
        sending.sendTrade(action, coinName, price, amount);

        qDebug() << coinName << action << ", 수량:" << amount;

        // 이 부분에서 서버나 매수/매도 처리 함수 호출
        // tradeManager->sendTradeRequest(coinSymbol, action, amount);
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
