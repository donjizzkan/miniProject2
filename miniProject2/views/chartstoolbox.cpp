#include "views/chartstoolbox.h"

#include <QDebug>

int lastXAxisUpdateSec = -60;

ChartsToolBox::ChartsToolBox(QWidget *parent)
    : QWidget{parent}
{
    // QToolBox 생성 - devwooms
    chartsToolBox = new QToolBox;
    // 화면 크기 자동으로 늘어나면 늘어나도록 Expanding - devwooms
    chartsToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // ChartsToolBox 위젯 자체에 layout 설정해서 chartsToolBox를 포함 - devwooms
    chartsToolBoxLayout = new QVBoxLayout(this);
    // 여백 제거 - devwooms
    chartsToolBoxLayout->setContentsMargins(0, 0, 0, 0);
    chartsToolBoxLayout->addWidget(chartsToolBox);
    setLayout(chartsToolBoxLayout);


    lineChartView = new LineChartView; // 멤버 변수로 저장 - devwooms
    candleChart = new CandleChart; // 멤버 변수로 저장 - devwooms

    // QToolBox 탭 부분에 Line Chart 추가 - devwooms
    chartsToolBox->addItem(lineChartView->lineChartView, "Line Chart");
    
    // QToolBox 탭 부분에 Candle Chart 추가 - devwooms
    chartsToolBox->addItem(candleChart->candleChartView, "Candle Chart");

}

LineChartView* ChartsToolBox::getLineChart(){
    return lineChartView;
}
CandleChart* ChartsToolBox::getCandleChart(){
    return candleChart;
}
