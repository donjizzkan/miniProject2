#include "singupview.h"
#include "ui_singupview.h"

SingupView::SingupView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SingupView)
{
    ui->setupUi(this);
}

SingupView::~SingupView()
{
    delete ui;
}
