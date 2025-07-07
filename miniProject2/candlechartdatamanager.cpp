#include "candlechartdatamanager.h"

CandleChartDataManager::CandleChartDataManager(QObject *parent)
    : QObject{parent}
{
    // 네트워크 매니저 생성 - Devwooms
    manager = new QNetworkAccessManager(this);
    // 응답 시 연결 - Devwooms
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply) {
        // 답이 에러인지 확인 후 모두 읽어 오기 - Devwooms
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument json = QJsonDocument::fromJson(responseData);
            dataFormat(json);
        } else {
            qWarning() << "API 요청 에러:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

// URL 및 HTTP 등등 세팅 - devwooms
void CandleChartDataManager::setting(QString coin)
{
    // 요청 URL 지정 - devwooms (캔들차트용 더 많은 데이터)
    QUrl apiUrl("https://api.bithumb.com/v1/candles/minutes/1?market="+coin+"&count=100");
    request.setUrl(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // HTTP/2 문제 해결을 위한 설정 - devwooms
    // 서버에 "정상적인 클라이언트입니다" 라고 알려줌 - Devwooms
    request.setHeader(QNetworkRequest::UserAgentHeader, "Qt Application");
    // HTTP/1.1 강제 사용 - Devwooms
    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, false);
    // HTTP 파이프라이닝은 여러 요청을 한번에 보내는 방식 - Devwooms
    // 순차 처리 강제 - Devwooms
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, false);
    // Keep-alive 비활성화 - Devwooms
    request.setRawHeader("Connection", "close");
}
// 데이터 요청 시작 -devwooms
void CandleChartDataManager::start()
{
    timer = new QTimer(this);
    timer->start(30000);
    // timer를 update에 연결 - Devwooms
    connect(timer, &QTimer::timeout, this, &CandleChartDataManager::update);
}
void CandleChartDataManager::update()
{
    manager->get(request);
}
void CandleChartDataManager::dataFormat(QJsonDocument json){
    QJsonArray dataArray = json.array();

    // 거래가와 시간 데이터를 담을 리스트 생성 - Devwooms
    QList<CandleData> candleDataList;

    // 각 거래 데이터 파싱 - Devwooms
    for (int i = 0; i < dataArray.size(); ++i) {
        QJsonObject serverData = dataArray[i].toObject();

        // 거래가와 시간 정보 추출 - Devwooms
        double openingPrice = serverData["opening_price"].toDouble();
        double highPrice = serverData["high_price"].toDouble();
        double lowPrice = serverData["low_price"].toDouble();
        double tradePrice = serverData["trade_price"].toDouble();
        double timestamp = serverData["timestamp"].toDouble();

        // CandleData로 데이터 저장 (4가지 가격과 시간) - Devwooms
        CandleData candleData(openingPrice, highPrice, lowPrice, tradePrice, timestamp);
        candleDataList.append(candleData);

    }

    // 데이터 전달 -> candleChart로 - Devwooms
    emit candleData(candleDataList);
}
