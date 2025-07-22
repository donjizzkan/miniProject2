#include "coinsearchwidget.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDebug>

CoinSearchWidget::CoinSearchWidget(QWidget *parent)
    : QWidget{parent}
{
    setupUI();
    initializeCoinData();
    connectSignal();
}


void CoinSearchWidget::initializeCoinData() {
    // 코인 데이터 초기화 (homeview.cpp의 comboBox 데이터와 동일)
    coinDataList.clear();
    
    coinDataList.append({"KRW-BTC", "비트코인", "Bitcoin", ":/assets/assets/Bitcoin.png"});
    coinDataList.append({"KRW-ETH", "이더리움", "Ethereum", ":/assets/assets/Ethereum.jpg"});
    coinDataList.append({"KRW-XRP", "엑스알피 [리플]", "XRP", ":/assets/assets/XRP.png"});
    coinDataList.append({"KRW-DOGE", "도지코인", "Dogecoin", ":/assets/assets/Dogecoin.svg"});
    coinDataList.append({"KRW-PEPE", "페페", "PEPE", ":/assets/assets/PEPE.jpg"});
}

void CoinSearchWidget::updateSearchText(const QString& text) {
    onSearchTextChanged(text);
}

void CoinSearchWidget::onSearchTextChanged(const QString& text) {
    qDebug() << "검색어 변경: " << text;
    // 타이핑할 때마다 즉시 필터링
    filterResults(text);
}

void CoinSearchWidget::addCoinToTable(const CoinData& coin) {
    int rowCount = resultTableWidget->rowCount();
    resultTableWidget->insertRow(rowCount);
    
    // 종, 한국어명, 영어명 순으로 데이터 추가
    resultTableWidget->setItem(rowCount, 0, new QTableWidgetItem(coin.symbol));
    resultTableWidget->setItem(rowCount, 1, new QTableWidgetItem(coin.koreanName));
    resultTableWidget->setItem(rowCount, 2, new QTableWidgetItem(coin.englishName));
    
    // 각 셀에 원본 데이터 저장 (나중에 선택시 사용)
    resultTableWidget->item(rowCount, 0)->setData(Qt::UserRole, coin.symbol);
    resultTableWidget->item(rowCount, 1)->setData(Qt::UserRole, coin.koreanName);
    resultTableWidget->item(rowCount, 2)->setData(Qt::UserRole, coin.englishName);
}

void CoinSearchWidget::onCoinSelected(QTableWidgetItem* item) {
    if (!item) return;
    
    int row = item->row();
    
    QString symbol = resultTableWidget->item(row, 0)->data(Qt::UserRole).toString();
    QString koreanName = resultTableWidget->item(row, 1)->data(Qt::UserRole).toString();
    QString englishName = resultTableWidget->item(row, 2)->data(Qt::UserRole).toString();
    
    qDebug() << "코인 선택됨:" << symbol << koreanName << englishName;
    
    // 선택된 코인 정보를 신호로 전달
    emit coinSelected(symbol, koreanName, englishName);
}

void CoinSearchWidget::filterResults(const QString& searchText) {
    resultTableWidget->setRowCount(0);

    if (searchText.isEmpty()) {
        return;
    }

    // 코인 데이터에서 검색어와 일치하는 항목 필터링
    for (const CoinData& coin : coinDataList) {
        // 여러 필드에서 검색 (종목, 한국어명, 영어명)
        if (coin.symbol.contains(searchText, Qt::CaseInsensitive) ||
            coin.koreanName.contains(searchText, Qt::CaseInsensitive) ||
            coin.englishName.contains(searchText, Qt::CaseInsensitive)) {
            
            addCoinToTable(coin);
        }
    }
}

void CoinSearchWidget::connectSignal(){
    // 테이블 아이템 클릭시 선택 이벤트
    connect(resultTableWidget, &QTableWidget::itemClicked, this, &CoinSearchWidget::onCoinSelected);
}

void CoinSearchWidget::setupUI(){
    // 메인 레이아웃 생성
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // 검색 결과를 보여줄 테이블 위젯 생성
    resultTableWidget = new QTableWidget(this);
    resultTableWidget->setColumnCount(3);
    
    // 테이블 헤더 설정
    QStringList headers;
    headers << "종목" << "한국어명" << "영어명";
    resultTableWidget->setHorizontalHeaderLabels(headers);
    
    // 테이블 설정
    resultTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    resultTableWidget->horizontalHeader()->setStretchLastSection(true);
    resultTableWidget->setAlternatingRowColors(true);
    resultTableWidget->setShowGrid(false);
    resultTableWidget->verticalHeader()->setVisible(false);
    
    // 테이블 크기 조정
    resultTableWidget->setColumnWidth(0, 80);
    resultTableWidget->setColumnWidth(1, 100);
    
    layout->addWidget(resultTableWidget);
    
    setLayout(layout);
}
