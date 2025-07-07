#ifndef CHARTSTOOLBOX_H
#define CHARTSTOOLBOX_H

#include "linechart.h"
#include "candlechart.h"

#include <QWidget>
#include <QtCharts>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QToolBox>

class LineChart; // forward declaration

class ChartsToolBox : public QWidget
{
    Q_OBJECT
public:
    explicit ChartsToolBox(QWidget *parent = nullptr);
    void getChartsNUM();
    LineChart* getLineChart(); // LineChart 인스턴스 접근 함수 - devwooms
    CandleChart* getCandleChart();

private:
    QJsonArray jsonData;

    QToolBox *chartsToolBox;
    QVBoxLayout *chartsToolBoxLayout;
    
    LineChart *lineChart; // LineChart 인스턴스 - devwooms
    CandleChart *candleChart; // LineChart 인스턴스 - devwooms

signals:
};

#endif // CHARTSTOOLBOX_H
