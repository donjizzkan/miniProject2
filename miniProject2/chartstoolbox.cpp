#include "chartstoolbox.h"

#include <QDebug>

int lastXAxisUpdateSec = -60;

ChartsToolBox::ChartsToolBox(QWidget *parent)
    : QWidget{parent}
{

    // ==============================
    // testdata - 시작 - devwooms
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
    // testdata -끝 - devwooms
    // ==============================

    // QToolBox 생성 - devwooms
    QToolBox *chartsToolBox = new QToolBox;
    // 화면 크기 자동으로 늘어나면 늘어나도록 Expanding - devwooms
    chartsToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    // QChart 라인 그래프 - devwooms
    chart = new QChart();
    jsonData = array;
    // 데이터 세팅 - devwooms
    chartData = chartDataSetting();
    
    // 데이터를 차트에 넣어주기 - devwooms
    chart->addSeries(chartData);
    
    // 차트 축 세팅 - devwooms
    chartSettingX();
    chartSettingY();


    // 차트 뷰 생성 - devwooms
    QChartView *chartView = new QChartView(chart);
    // 안티앨리어싱 옵션 선택 ( 부드럽게 ) - devwooms
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // QToolBox 탭 부분에 Line Chart 추가 - devwooms
    chartsToolBox->addItem(chartView, "Line Chart");

    // ChartsToolBox 위젯 자체에 layout 설정해서 chartsToolBox를 포함 - devwooms
    QVBoxLayout *layout = new QVBoxLayout(this);
    // 여백 제거 - devwooms
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartsToolBox);
    setLayout(layout);
    
    // 업데이트 - devwooms
    chartUpdate();
}

// 데이터 세팅 - devwooms
QLineSeries* ChartsToolBox::chartDataSetting(){
    QLineSeries *chartData = new QLineSeries();
    chartData->setName("비트 코인");

    // JSON 배열 반복하면서 값 추가 - devwooms
    for (int i = 0; i < jsonData.size(); ++i) {
        const QJsonObject obj = jsonData[i].toObject();

        // X축  - devwooms
        int today = obj["today"].toInt();
        // Y축  - devwooms
        double price = obj["price"].toDouble();

        // x축 : 날짜, y축 : 가격 - devwooms
        chartData->append(today, price);
    }

    return chartData;
}

// x축 설정 - devwooms
void ChartsToolBox::chartSettingX(){
    // 안전하게 축 처리 - 기존 축이 있으면 범위만 업데이트 - devwooms
    QList<QAbstractAxis*> axes = chart->axes(Qt::Horizontal);
    if (!axes.isEmpty()) {
        QCategoryAxis *existingAxisX = qobject_cast<QCategoryAxis*>(axes.first());
        if (existingAxisX) {
            // 기존 축의 범위만 업데이트 - devwooms
            QTime now = QTime::currentTime();
            int nowSecs = QTime(0, 0).secsTo(now);
            //  전부터 현재까지 - devwooms
            int end = nowSecs;
            int start = qMax(0, end - 1 * 100);
            existingAxisX->setRange(start, end);
            return;
        }
    }

    // 처음 생성하는 경우에만 새 축 생성 - devwooms
    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setLabelsAngle(-90);
    // 현재 시간 - devwooms
    QTime now = QTime::currentTime();
    int nowSecs = QTime(0, 0).secsTo(now);
    //  전부터 현재까지 - devwooms
    int end = nowSecs;
    int start = qMax(0, end - 1 * 100);

    // 범위 넣어주기 - devwooms
    axisX->setRange(start, end);

    // X축 그래프 분류 넣어주기 - devwooms
    for (int s = start; s <= end; s += 100) {
        QString label = QTime(0, 0).addSecs(s).toString("HH:mm");
        axisX->append(label, s);
    }

    chart->addAxis(axisX, Qt::AlignBottom);
    chartData->attachAxis(axisX);
}

// y축 설정 - devwooms
void ChartsToolBox::chartSettingY(){
    if (!chartData || chartData->points().isEmpty())
        return;

    // chartData에서 최소/최대 y값 찾기 - devwooms
    qreal minY = std::numeric_limits<qreal>::max();
    qreal maxY = std::numeric_limits<qreal>::lowest();

    const QVector<QPointF> &points = chartData->points();
    for (const QPointF &point : points) {
        minY = qMin(minY, point.y());
        maxY = qMax(maxY, point.y());
    }

    const qreal margin = 2.0;
    qreal lower = minY - margin;
    qreal upper = maxY + margin;

    // 이미 존재하는 Y축이 있으면 setRange만 갱신 - devwooms
    QList<QAbstractAxis*> axes = chart->axes(Qt::Vertical);
    for (QAbstractAxis *axis : std::as_const(axes)) {
        QValueAxis *valueAxis = qobject_cast<QValueAxis*>(axis);
        if (valueAxis) {
            valueAxis->setRange(lower, upper);
            return;
        }
    }

    // Y축이 없을 경우 새로 생성 - devwooms
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(lower, upper);
    chart->addAxis(axisY, Qt::AlignLeft);
    chartData->attachAxis(axisY);
}

void ChartsToolBox::chartUpdate(){
    // 타이머로 실시간 x축 업데이트 - devwooms
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() {
        // 현재 시간 기준으로 x축 범위만 업데이트 - devwooms
        QTime now = QTime::currentTime();
        int nowSecs = QTime(0, 0).secsTo(now);
        // int start = qMax(0, nowSecs - 1 * 3600 /60);
        // int end = qMin(86399, nowSecs + 1 * 3600 /60);
        int end = nowSecs;
        int start = qMax(0, end - 1 * 100);

        // 데이터 업데이트 - devwooms
        QList<QPointF> points;
        for (const auto &val : std::as_const(jsonData)) {
            QJsonObject obj = val.toObject();
            int t = obj["today"].toInt();
            double p = obj["price"].toDouble();
            if (t >= start && t <= end) {
                points.append(QPointF(t, p));
            }
        }
        chartData->replace(points);
        
        // x축이 있는지 확인하고 범위만 업데이트 - devwooms
        QList<QAbstractAxis*> axes = chart->axes(Qt::Horizontal);
        if (!axes.isEmpty()) {
            QCategoryAxis *axisX = qobject_cast<QCategoryAxis*>(axes.first());
            if (axisX) {
                axisX->setRange(start, end);
            }
        }



        // Y도 갱신 - devwooms
        chartSettingY();
    });

    // 1초 마다 실행  - devwooms
    updateTimer->start(1000);
}
