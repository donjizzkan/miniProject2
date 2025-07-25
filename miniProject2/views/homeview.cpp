#include "homeview.h"
#include "linechartview.h"
#include "models/sendingManage.h"
#include "chartstoolbox.h"
#include "models/chattinglistmanager.h"
#include <QIcon>
#include <QSizePolicy>
#include <QDebug>

HomeView::HomeView(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    connectSignal();

    // 소켓에서 거래완료 신호 받을 경우 할 행동 호출
    connect(&SocketManage::instance(), &SocketManage::tradeResponseReceived, this, &HomeView::handleTradeResponse);

    // ==============================
    // 거래 탭 - devwooms
    // ==============================

    // ChartsToolBox 인스턴스 생성 - devwooms
    chartBox = new ChartsToolBox(this); // 멤버 변수로 저장 - devwooms
    chartBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    comboBox->addItem(QIcon(":/assets/assets/Bitcoin.png"), "KRW-BTC / 비트코인 / Bitcoin");
    comboBox->addItem(QIcon(":/assets/assets/Ethereum.jpg"), "KRW-ETH / 이더리움 / Ethereum");
    comboBox->addItem(QIcon(":/assets/assets/XRP.png"), "KRW-XRP / 엑스알피 [리플] / XRP");
    comboBox->addItem(QIcon(":/assets/assets/Dogecoin.svg"), "KRW-DOGE / 도지코인 / Dogecoin");
    comboBox->addItem(QIcon(":/assets/assets/PEPE.jpg"), "KRW-PEPE / 페페 / PEPE");



    // tab의 layout에 추가 - devwooms
    chartTab->addWidget(chartBox);

    // ==============================
    // 오른쪽 채팅 ToolBox - devwooms
    // ==============================

    ChattingListManager *chattingListManager = new ChattingListManager;

    chattingListManager->settingChattingToolBox(chatting_ToolBox);

    chattingListManager->settingConnectListWidget(connect_listWidget);
    chattingListManager->settingOneByOneListWidget(oneByone_listWidget);
    chattingListManager->settingOneByMoreListWidget(oneByMore_listWidget);
}

// 특정 이벤트 필터 ( 전체적인 widget의 override임
bool HomeView::eventFilter(QObject *obj, QEvent *event) {
    qDebug() << "eventFilter 호출 - obj:" << obj << "event type:" << event->type();
    
    // obj가 lineEdit일 때
    if (obj == searchLineEdit) {
        qDebug() << "searchLineEdit 이벤트 감지";
        
        if (event->type() == QEvent::FocusIn) {
            qDebug() << "Focus In - coinSearchWidget 보이기";
            coinSearchWidget->show();
            coinSearchWidget->raise(); // 위에 나타나도록
            return true;
        } 
        else if (event->type() == QEvent::FocusOut) {
            qDebug() << "Focus Out - coinSearchWidget 숨기기";
            // 약간의 지연을 두어 다른 위젯이 포커스를 받을 수 있도록 함
            QTimer::singleShot(100, this, [this]() {
                if (!searchLineEdit->hasFocus() && !coinSearchWidget->hasFocus()) {
                    coinSearchWidget->hide();
                }
            });
            return true;
        // 키보드 입력시
        } else if (event->type() == QEvent::KeyPress){
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                qDebug() << "Enter 키 감지";
                QString searchText = searchLineEdit->text();

                if (!searchText.isEmpty()) {
                    // Enter 입력 시 동작
                    // coinSearchWidget->selectFirstResult();
                    coinSearchWidget->hide();
                    searchLineEdit->clearFocus();
                }
                return true; // 이벤트 처리 완료
            }


        }
    }
    
    return QWidget::eventFilter(obj, event);
}

void HomeView::connectSignal(){

    connect(pushButton, &QPushButton::clicked, this, [this]() {
        QString action;
        int amount = 0;     // 수량

        if (radioButton->isChecked()) {
            action = "buy";
            amount = spinBox->value();
            spinBox->setValue(0);

        } else if (radioButton_2->isChecked()) {
            action = "sell";
            amount = spinBox_2->value();
            spinBox_2->setValue(0);
        }

        // 콤보박스에서 코인 심볼(krw-btc 등) 추출
        QString comboText = comboBox->itemText(comboBox->currentIndex());
        QString coinName = comboText.split(" / ").first().toLower();

        // 최신 가격 불러오기 (네가 chartBox에 해당 함수 구현해놨다면)
        double price = chartBox->getLineChart()->getLatestPrice(); // 또는 getCurrentPrice()

        // 거래 요청 보내기 (싱글턴 사용 권장)
        sendingManage::instance()->sendTrade(action, coinName, price, amount);

        qDebug() << "거래신호 전송:" << coinName << action << ", 수량:" << amount << ", 가격:" << price;
    });


    // comboBox에서 선택시 선택 코인의 데이터를 가져오도록 연결
    connect(comboBox, &QComboBox::currentIndexChanged, this, [this](int index) {
        QString text = comboBox->itemText(index);
        qDebug() << "선택된 항목:" << text;

        // 코인 종목 추출 (예: "KRW-BTC / 비트코인 / Bitcoin" → "krw-btc") - devwooms
        QString coinSymbol = text.split(" / ").first().toLower();
        qDebug() << "추출된 코인 종목:" << coinSymbol;

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
    
    // searchLineEdit 텍스트 변경시 CoinSearchWidget에 전달
    connect(searchLineEdit, &QLineEdit::textChanged, coinSearchWidget, &CoinSearchWidget::updateSearchText);
    
    // CoinSearchWidget에서 코인 선택시 comboBox 업데이트
    connect(coinSearchWidget, &CoinSearchWidget::coinSelected, this, [this](const QString& symbol, const QString& koreanName, const QString& englishName) {
        qDebug() << "코인 선택됨 - 종목:" << symbol << "한국어명:" << koreanName << "영어명:" << englishName;
        
        // comboBox에서 해당 코인 찾아서 선택
        QString targetText = QString("%1 / %2 / %3").arg(symbol, koreanName, englishName);
        
        for (int i = 0; i < comboBox->count(); ++i) {
            if (comboBox->itemText(i) == targetText) {
                comboBox->setCurrentIndex(i);
                break;
            }
        }
        
        // 검색 위젯 숨기기
        coinSearchWidget->hide();
        searchLineEdit->clearFocus();
    });
}

void HomeView::setupUI()
{
    // 메인 레이아웃
    QHBoxLayout *horizontalLayout_4 = new QHBoxLayout(this);
    horizontalLayout_4->setContentsMargins(5, 5, 5, 5);
    
    // 스플리터
    splitter = new QSplitter(Qt::Horizontal);
    
    // 왼쪽 탭 위젯
    tapwidget = new QTabWidget();
    tapwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // 시세 탭
    QWidget *tab = new QWidget();
    chartTab = new QVBoxLayout(tab);


    // 코인 검색 기능 추가
    searchLineEdit = new QLineEdit();
    searchLineEdit->setPlaceholderText("코인 검색...");
    searchLineEdit->setClearButtonEnabled(true); // X 버튼 추가
    // 이벤트 필터 설치
    searchLineEdit->installEventFilter(this);
    chartTab->addWidget(searchLineEdit);

    // 검색 결과 위젯
    coinSearchWidget = new CoinSearchWidget(this);
    coinSearchWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    coinSearchWidget->setMinimumHeight(100);
    coinSearchWidget->hide(); // 초기에는 숨김
    chartTab->addWidget(coinSearchWidget);

    // 드롭다운
    comboBox = new QComboBox();
    chartTab->addWidget(comboBox);
    

    

    tapwidget->addTab(tab, "시세");

    // 계좌 탭
    QWidget *tab_2 = new QWidget();
    QVBoxLayout *verticalLayout = new QVBoxLayout(tab_2);
    
    // 상단 텍스트 브라우저
    textBrowser = new QTextBrowser();
    textBrowser->setHtml("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                         "<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
                         "p, li { white-space: pre-wrap; }\n"
                         "hr { height: 1px; border-width: 0; }\n"
                         "li.unchecked::marker { content: \"\2610\"; }\n"
                         "li.checked::marker { content: \"\2612\"; }\n"
                         "</style></head><body style=\" font-family:'.AppleSystemUIFont'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
                         "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'맑은 고딕'; font-size:9pt;\">이름 / &lt;현재 보유중인 현금&gt;</span></p></body></html>");
    verticalLayout->addWidget(textBrowser);
    
    // 그리드 레이아웃
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    
    // 그리드 요소들 추가
    purchasePrice = new QTextBrowser();
    gridLayout->addWidget(purchasePrice, 0, 1, 1, 2);
    
    QTextBrowser *textBrowser_8 = new QTextBrowser();
    textBrowser_8->setHtml("<p align=\"center\">수익률(%)</p>");
    gridLayout->addWidget(textBrowser_8, 1, 3);
    
    QTextBrowser *textBrowser_6 = new QTextBrowser();
    textBrowser_6->setHtml("<p align=\"center\">평가손익</p>");
    gridLayout->addWidget(textBrowser_6, 0, 3);
    
    QTextBrowser *textBrowser_4 = new QTextBrowser();
    textBrowser_4->setHtml("<p align=\"center\">평가금액</p>");
    gridLayout->addWidget(textBrowser_4, 1, 0, 1, 2);
    
    gainPercent = new QTextBrowser();
    gridLayout->addWidget(gainPercent, 1, 4);
    
    gain = new QTextBrowser();
    gridLayout->addWidget(gain, 0, 4);
    
    QTextBrowser *textBrowser_3 = new QTextBrowser();
    textBrowser_3->setHtml("<p align=\"center\">매입금액</p>");
    gridLayout->addWidget(textBrowser_3, 0, 0);
    
    recentPrice = new QTextBrowser();
    gridLayout->addWidget(recentPrice, 1, 2);
    
    verticalLayout->addLayout(gridLayout);
    
    // 구분선
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    verticalLayout->addWidget(line);
    
    // 거래내역 제목
    QTextBrowser *textBrowser_23 = new QTextBrowser();
    textBrowser_23->setHtml("<p align=\"center\">거래내역</p>");
    verticalLayout->addWidget(textBrowser_23);
    
    // 거래내역 헤더
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(0);
    
    QTextBrowser *textBrowser_14 = new QTextBrowser();
    textBrowser_14->setHtml("<p align=\"center\">주문정보</p>");
    horizontalLayout->addWidget(textBrowser_14);
    
    QTextBrowser *textBrowser_10 = new QTextBrowser();
    textBrowser_10->setHtml("<p align=\"center\">거래일시</p>");
    horizontalLayout->addWidget(textBrowser_10);
    
    QTextBrowser *textBrowser_11 = new QTextBrowser();
    textBrowser_11->setHtml("<p align=\"center\">거래수량</p>");
    horizontalLayout->addWidget(textBrowser_11);
    
    QTextBrowser *textBrowser_12 = new QTextBrowser();
    textBrowser_12->setHtml("<p align=\"center\">거래액수</p>");
    horizontalLayout->addWidget(textBrowser_12);
    
    QTextBrowser *textBrowser_13 = new QTextBrowser();
    textBrowser_13->setHtml("<p align=\"center\">총액수</p>");
    horizontalLayout->addWidget(textBrowser_13);
    
    verticalLayout->addLayout(horizontalLayout);
    
    // 거래내역 데이터
    QHBoxLayout *horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setSpacing(0);
    
    orderType = new QTextBrowser();
    horizontalLayout_3->addWidget(orderType);
    
    orderDate = new QTextBrowser();
    horizontalLayout_3->addWidget(orderDate);
    
    orderAmount = new QTextBrowser();
    horizontalLayout_3->addWidget(orderAmount);
    
    orderPrice = new QTextBrowser();
    horizontalLayout_3->addWidget(orderPrice);
    
    orderTotal = new QTextBrowser();
    horizontalLayout_3->addWidget(orderTotal);
    
    verticalLayout->addLayout(horizontalLayout_3);
    
    tapwidget->addTab(tab_2, "계좌");
    
    splitter->addWidget(tapwidget);
    
    // 오른쪽 탭 위젯
    QTabWidget *tabWidget = new QTabWidget();
    
    // 매수/매도 탭
    QWidget *tab_3 = new QWidget();
    QWidget *layoutWidget = new QWidget(tab_3);
    layoutWidget->setGeometry(QRect(21, 13, 213, 427));
    
    QVBoxLayout *verticalLayout_4 = new QVBoxLayout(layoutWidget);
    
    // 여백
    QSpacerItem *verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout_4->addItem(verticalSpacer_2);
    
    // 현재 잔고
    QLabel *label = new QLabel("현재 잔고");
    label->setAlignment(Qt::AlignCenter);
    verticalLayout_4->addWidget(label);
    
    lineEdit = new QLineEdit();
    lineEdit->setMinimumHeight(30);
    lineEdit->setReadOnly(true);
    verticalLayout_4->addWidget(lineEdit);
    
    QSpacerItem *verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout_4->addItem(verticalSpacer_4);
    
    // 매수/매도 옵션
    QVBoxLayout *verticalLayout_3 = new QVBoxLayout();
    
    radioButton = new QRadioButton("매수");
    radioButton->setMinimumHeight(30);
    QFont font;
    font.setPointSize(20);
    radioButton->setFont(font);
    verticalLayout_3->addWidget(radioButton);
    
    spinBox = new QSpinBox();
    spinBox->setMinimumHeight(30);
    spinBox->setMaximum(99999);
    verticalLayout_3->addWidget(spinBox);
    
    radioButton_2 = new QRadioButton("매도");
    radioButton_2->setMinimumHeight(30);
    radioButton_2->setFont(font);
    verticalLayout_3->addWidget(radioButton_2);
    
    spinBox_2 = new QSpinBox();
    spinBox_2->setMinimumHeight(30);
    spinBox_2->setMaximum(99999);
    verticalLayout_3->addWidget(spinBox_2);
    
    verticalLayout_4->addLayout(verticalLayout_3);
    
    QSpacerItem *verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout_4->addItem(verticalSpacer_3);
    
    // 버튼 레이아웃
    QHBoxLayout *horizontalLayout_5 = new QHBoxLayout();
    QSpacerItem *horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_5->addItem(horizontalSpacer_2);
    
    pushButton = new QPushButton("PushButton");
    pushButton->setMinimumHeight(50);
    horizontalLayout_5->addWidget(pushButton);
    
    QSpacerItem *horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_5->addItem(horizontalSpacer_3);
    
    verticalLayout_4->addLayout(horizontalLayout_5);
    
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout_4->addItem(verticalSpacer);
    
    tabWidget->addTab(tab_3, "매수 / 매도");
    
    // 채팅 탭
    QWidget *tab_4 = new QWidget();
    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout(tab_4);
    
    chatting_ToolBox = new QToolBox();
    chatting_ToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chatting_ToolBox->setMinimumWidth(200);
    chatting_ToolBox->setCurrentIndex(2);
    
    // 접속자 목록
    QWidget *connect_list = new QWidget();
    connect_list->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *verticalLayout_2 = new QVBoxLayout(connect_list);
    connect_listWidget = new QListWidget();
    verticalLayout_2->addWidget(connect_listWidget);
    chatting_ToolBox->addItem(connect_list, "접속자 목록");
    
    // 개인 채팅방
    QWidget *oneByone_list = new QWidget();
    QVBoxLayout *verticalLayout_6 = new QVBoxLayout(oneByone_list);
    verticalLayout_6->setContentsMargins(0, 0, 0, 0);
    oneByone_listWidget = new QListWidget();
    verticalLayout_6->addWidget(oneByone_listWidget);
    chatting_ToolBox->addItem(oneByone_list, "개인 채팅방");
    
    // 오픈 채팅방
    QWidget *oneByMore_list = new QWidget();
    QVBoxLayout *verticalLayout_7 = new QVBoxLayout(oneByMore_list);
    verticalLayout_7->setContentsMargins(0, 0, 0, 0);
    oneByMore_listWidget = new QListWidget();
    verticalLayout_7->addWidget(oneByMore_listWidget);
    chatting_ToolBox->addItem(oneByMore_list, "오픈 채팅방");
    
    horizontalLayout_2->addWidget(chatting_ToolBox);
    
    tabWidget->addTab(tab_4, "채팅");
    
    splitter->addWidget(tabWidget);
    
    horizontalLayout_4->addWidget(splitter);
    
    // 오른쪽 여백
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_4->addItem(horizontalSpacer);
}

void HomeView::handleTradeResponse(const QJsonObject &obj){
    // [수정된 부분] obj 전체가 아니라 user 키 안의 객체를 먼저 꺼내야 함 -> 이제 obj에서 바로 받음
    // QJsonObject userInfo = obj["user"].toObject(); // 이제 필요 없음

    qDebug() << "받은 tradeResponse 전체:" << obj; // 이제 obj에 money, payment, coins가 직접 담김
    // qDebug() << "userInfo 전체:" << userInfo; // 이제 필요 없음
    qDebug() << "payment raw:" << obj["payment"]; // [수정된 부분] obj에서 바로 payment를 확인
    qDebug() << "payment toDouble:" << obj["payment"].toDouble();
    qDebug() << "money raw:" << obj["money"]; // [수정된 부분] obj에서 바로 money를 확인
    qDebug() << "money toDouble:" << obj["money"].toDouble();


    QString result = obj["result"].toString();
    if(result != "success"){
        textBrowser->append("거래 실패, 잔고를 확인하세요");
        return;
    }

    // [수정된 부분] money, payment, coins를 obj에서 직접 받음
    double money = obj["money"].toDouble();
    double payment = obj["payment"].toDouble();
    QJsonObject coins = obj["coins"].toObject(); // [수정된 부분] obj에서 바로 coins를 받음

    QString selectedCoin = comboBox->currentText().split(" / ").first().toLower();

    int coinAmount = coins[selectedCoin].toInt();

    // 최신 거래 가격 (history 마지막 값에서 price 추출)
    QJsonArray history = obj["history"].toArray(); // history는 원래 obj에서 직접 받음

    double latestPrice = 0;
    if(!history.isEmpty()) {
        QJsonObject lastRecord = history.last().toObject();
        latestPrice = lastRecord["price"].toDouble();
    }

    // 평가금액 = 현재 보유수량 * 최신가격
    double evalAmount = coinAmount * latestPrice;
    // 평가손익 = 평가금액 - payment (단순화, 실제 계산 로직은 상황에 따라 보정)
    double evalProfit = evalAmount - payment;
    // 수익률 = (평가손익 / payment) * 100
    double profitPercent = payment > 0 ? (evalProfit / payment) * 100.0 : 0;

    purchasePrice->setText(QString::number(payment, 'f', 2));      // 매입금액
    gain->setText(QString::number(evalProfit, 'f', 2));            // 평가손익
    recentPrice->setText(QString::number(evalAmount, 'f', 2));     // 평가금액
    gainPercent->setText(QString::number(profitPercent, 'f', 2));  // 수익률(%)

    orderType->clear();
    orderDate->clear();
    orderAmount->clear();
    orderPrice->clear();
    orderTotal->clear();
    textBrowser->setHtml(QString("<p align=\"center\">계좌 잔고 : %1</p>").arg(QString::number(money, 'f', 2)));

    for (const auto &val : history) {
        QJsonObject rec = val.toObject();

        // 주문정보: 매수/매도 + 코인명
        QString orderInfo = QString("%1(%2)").arg(rec["action"].toString()).arg(rec["coin"].toString());
        // 거래일시: datetime
        QString date = rec["datetime"].toString();
        // 거래수량: amount
        QString amount = QString::number(rec["amount"].toInt());
        // 거래액수: price
        QString price = QString::number(rec["price"].toDouble());
        // 총액수: price * amount
        QString total = QString::number(rec["price"].toDouble() * rec["amount"].toInt());

        orderType->append(orderInfo);
        orderDate->append(date);
        orderAmount->append(amount);
        orderPrice->append(price);
        orderTotal->append(total);
    }
}

void HomeView::setAccountInfo(const QJsonObject &userInfo, const QJsonArray &history) {
    // userInfo는 이미 user 키 안의 객체로 전달된 상태라고 가정
    qDebug() << "userInfo 전체:" << userInfo;
    qDebug() << "payment raw:" << userInfo["payment"];
    qDebug() << "payment toDouble:" << userInfo["payment"].toDouble();
    qDebug() << "money raw:" << userInfo["money"];
    qDebug() << "money toDouble:" << userInfo["money"].toDouble();

    double money = userInfo["money"].toDouble();
    double payment = userInfo["payment"].toDouble();
    QJsonObject coins = userInfo["coins"].toObject();
    QString selectedCoin = comboBox->currentText().split(" / ").first().toLower();
    int coinAmount = coins[selectedCoin].toInt();

    // 거래내역 최신가 구하기
    double latestPrice = 0;
    if (!history.isEmpty()) {
        QJsonObject lastRec = history.last().toObject();
        latestPrice = lastRec["price"].toDouble();
    }

    double evalAmount = coinAmount * latestPrice;
    double evalProfit = evalAmount - payment;
    double profitPercent = payment > 0 ? (evalProfit / payment) * 100.0 : 0;

    // 각 칸 채우기
    purchasePrice->setText(QString::number(payment, 'f', 2));
    gain->setText(QString::number(evalProfit, 'f', 2));
    recentPrice->setText(QString::number(evalAmount, 'f', 2));
    gainPercent->setText(QString::number(profitPercent, 'f', 2));
    textBrowser->setHtml(QString("<p align=\"center\">계좌 잔고 : %1</p>").arg(QString::number(money, 'f', 2)));

    // 거래내역 표 채우기
    orderType->clear();
    orderDate->clear();
    orderAmount->clear();
    orderPrice->clear();
    orderTotal->clear();
    for (const auto &val : history) {
        QJsonObject rec = val.toObject();
        QString orderInfo = QString("%1(%2)").arg(rec["action"].toString()).arg(rec["coin"].toString());
        QString date = rec["datetime"].toString();
        QString amount = QString::number(rec["amount"].toInt());
        QString price = QString::number(rec["price"].toDouble());
        QString total = QString::number(rec["price"].toDouble() * rec["amount"].toInt());
        orderType->append(orderInfo);
        orderDate->append(date);
        orderAmount->append(amount);
        orderPrice->append(price);
        orderTotal->append(total);
    }
}


HomeView::~HomeView()
{
}
