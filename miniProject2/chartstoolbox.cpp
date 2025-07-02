#include "chartstoolbox.h"

#include <QToolBox>

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

ChartsToolBox::ChartsToolBox(QWidget *parent)
    : QWidget{parent}
{

    // ==============================
    // testdata - 시작
    // ==============================

    //
    QFile file(":/charts/test/btc_price.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("파일 열기 실패");
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        qWarning("JSON 배열이 아님");
    }

    QJsonArray array = doc.array();

    // ==============================
    // testdata -끝
    // ==============================

    // QToolBox 생성
    QToolBox *chartsToolBox = new QToolBox;
    chartsToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    // QChart 라인 그래프 생성
    QChart *chart = new QChart();
    chartData = chartDataSetting();


    // JSON 배열 반복하면서 값 추가
    for (int i = 0; i < array.size(); ++i) {
        const QJsonObject obj = array[i].toObject();

        int today = obj["today"].toInt();   // x값
        double price = obj["price"].toDouble();    // y값

        // x축 : 날짜, y축 : 가격
        chartData->append(today, price);
    }


    // 차트 축 세팅
    chartSettingX(*chart, chartData);
    chartSettingY(*chart, chartData);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);



    // QToolBox에 탭 추가
    chartsToolBox->addItem(chartView, "Line Chart");

    chart -> addSeries(chartData);


    // ChartsToolBox 위젯 자체에 layout 설정해서 chartsToolBox를 포함
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);  // 여백 제거
    layout->addWidget(chartsToolBox);
    setLayout(layout);
}

// 데이터 세팅
QLineSeries* ChartsToolBox::chartDataSetting(){
    QLineSeries *chartData = new QLineSeries();

    return chartData;
}

// x축 설정
void ChartsToolBox::chartSettingX(QChart &chart, QLineSeries *chartData){

    QCategoryAxis *axisX = new QCategoryAxis();
    // 1시간 간격
    axisX->append("00:00", 0);
    axisX->append("01:00", 3600);
    axisX->append("02:00", 7200);
    axisX->append("03:00", 10800);
    axisX->append("04:00", 14400);
    axisX->append("05:00", 18000);
    axisX->append("06:00", 21600);
    axisX->append("07:00", 25200);
    axisX->append("08:00", 28800);
    axisX->append("09:00", 32400);
    axisX->append("10:00", 36000);
    axisX->append("11:00", 39600);
    axisX->append("12:00", 43200);
    axisX->append("13:00", 46800);
    axisX->append("14:00", 50400);
    axisX->append("15:00", 54000);
    axisX->append("16:00", 57600);
    axisX->append("17:00", 61200);
    axisX->append("18:00", 64800);
    axisX->append("19:00", 68400);
    axisX->append("20:00", 72000);
    axisX->append("21:00", 75600);
    axisX->append("22:00", 79200);
    axisX->append("23:00", 82800);
    axisX->append("00:00", 86399);
    axisX->setRange(0, 86400);
    // axisX->setTitleText("시간");

    chart.addAxis(axisX, Qt::AlignBottom);
    chartData->attachAxis(axisX);
}

// y축 설정
void ChartsToolBox::chartSettingY(QChart &chart, QLineSeries *chartData){
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(28000, 30000);
    // axisY->setTitleText("가격");


    chart.addAxis(axisY, Qt::AlignLeft);
    chartData->attachAxis(axisY);
}
