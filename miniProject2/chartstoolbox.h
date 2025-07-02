#ifndef CHARTSTOOLBOX_H
#define CHARTSTOOLBOX_H

#include <QWidget>
#include <QtCharts>

class ChartsToolBox : public QWidget
{
    Q_OBJECT
public:
    explicit ChartsToolBox(QWidget *parent = nullptr);
    void getChartsNUM();

private:
    QLineSeries *chartData;
    QJsonArray jsonData;



    QLineSeries* chartDataSetting();
    void chartSettingX(QChart &chart, QLineSeries *chartData);
    void chartSettingY(QChart &chart, QLineSeries *chartData);

signals:
};

#endif // CHARTSTOOLBOX_H
