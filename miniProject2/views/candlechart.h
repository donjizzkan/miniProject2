#ifndef CANDLECHART_H
#define CANDLECHART_H

#include "models/candlechartdatamanager.h"

#include <QObject>
#include <QTime>
#include <QChartView>
#include <QChart>
#include <QCandlestickSeries>
#include <QLineSeries>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QDateTime>

class CandleChart : public QObject
{
    Q_OBJECT
public:
    explicit CandleChart(QObject *parent = nullptr);
    QChartView *candleChartView;
    QString candleCoin;
    void setCoin(const QString &coin);
    CandleChartDataManager *candleDataManager;

public slots:
    void getCandleData(const QList<CandleData> &candleDataList);

private:

    QChart *candleChart;
    QCandlestickSeries *candleSeries;
    QCandlestickSeries *createCandleSeries();
    void setupCandleChartAxes();
    void updateCandleChart(int start, int end, double price);
    void updateCandleData(const QList<CandleData> &candleDataList);

signals:
};

#endif // CANDLECHART_H
