#ifndef LINECHART_H
#define LINECHART_H

#include <QObject>

#include "linechartdatamanager.h"

#include <QTime>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QDateTime>

class LineChart : public QObject
{
    Q_OBJECT
public:
    explicit LineChart(QObject *parent = nullptr);
    QChartView *lineChartView;
    QString lineCoin;
    void setCoin(const QString &coin);
    LineChartDataManager *lineDataManager;

public slots:
    void getLineData(const QList<QPointF> &lineDataList);

private:
    QChart *lineChart;
    QLineSeries *lineSeries;
    QLineSeries *createLineSeries();
    void setupLineChartAxes();
    void updateLineChart(int start, int end, double price);
    void updateLineData(const QList<QPointF> &lineDataList);

signals:
};

#endif // LINECHART_H
