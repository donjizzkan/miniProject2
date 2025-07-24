#ifndef LINECHARTVIEW_H
#define LINECHARTVIEW_H

#include <QObject>

#include "models/linechartdatamanager.h"

#include <QTime>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QDateTime>

class LineChartView : public QObject
{
    Q_OBJECT
public:
    explicit LineChartView(QObject *parent = nullptr);
    QChartView *lineChartView;
    QString lineCoin;
    void setCoin(const QString &coin);
    LineChartDataManager *lineDataManager;
    double getLastPrice() const {return latestPrice;}

public slots:
    void getLineData(const QList<QPointF> &lineDataList);

private:
    QChart *lineChart;
    QLineSeries *lineSeries;
    QLineSeries *createLineSeries();
    void setupLineChartAxes();
    void updateLineChart(int start, int end, double price);
    void updateLineData(const QList<QPointF> &lineDataList);
    double latestPrice = 0.0;

signals:
};

#endif // LINECHARTVIEW_H
