#include "charts.h"
#include "ui_charts.h"

Charts::Charts(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Charts)
{
    ui->setupUi(this);
}

Charts::~Charts()
{
    delete ui;
}
