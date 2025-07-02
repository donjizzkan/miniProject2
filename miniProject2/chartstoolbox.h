#ifndef CHARTSTOOLBOX_H
#define CHARTSTOOLBOX_H

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

class ChartsToolBox : public QWidget
{
    Q_OBJECT
public:
    explicit ChartsToolBox(QWidget *parent = nullptr);
    void getChartsNUM();

private:
    QChart *chart;
    QLineSeries *chartData;
    QJsonArray jsonData;
    QTimer *updateTimer;

    QLineSeries* chartDataSetting();
    void chartSettingX();
    void chartSettingY();

    // 실시간 차트 업데이트
    void chartUpdate();

signals:
};

#endif // CHARTSTOOLBOX_H
