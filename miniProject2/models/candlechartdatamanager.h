#ifndef CANDLECHARTDATAMANAGER_H
#define CANDLECHARTDATAMANAGER_H

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPointF>
#include <QList>

struct CandleData {
    double open;
    double high;
    double low;
    double trade;
    double time;
    // 생성자 추가
    CandleData(double o, double h, double l, double t, double dt)
        : open(o), high(h), low(l), trade(t), time(dt) {}
};

class CandleChartDataManager : public QObject
{
    Q_OBJECT
public:
    explicit CandleChartDataManager(QObject *parent = nullptr);

public slots:
    void setting(QString coin);
    void start();
    void update();
    void dataFormat(QJsonDocument json);
private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QTimer *timer;
signals:
    void candleData(QList<CandleData> candleDataList);
};

#endif // CANDLECHARTDATAMANAGER_H
