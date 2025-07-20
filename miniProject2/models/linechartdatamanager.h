#ifndef LINECHARTDATAMANAGER_H
#define LINECHARTDATAMANAGER_H

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPointF>
#include <QList>

class LineChartDataManager : public QObject
{
    Q_OBJECT
public:
    explicit LineChartDataManager(QObject *parent = nullptr);

    // 최근 값 getter
    double getLatestPrice() const;

public slots:
    void setting(QString coin);
    void start();
    void update();
    void dataFormat(QJsonDocument json);
private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QTimer *timer;

    // 최근 값을 멤버 변수로 저장
    double latestPrice = 0.0;
signals:
    // QPointF를 쓰는 이유는
    // Json으로 다시 만들 경우 소모가 크고
    // X, Y로 표현하기 좋아서 시간, 가격
    void lineData(QList<QPointF> lineData);
};

#endif // LINECHARTDATAMANAGER_H
