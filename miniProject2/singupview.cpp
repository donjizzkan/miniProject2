#include "singupview.h"
#include "ui_singupview.h"

SingupView::SingupView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SingupView)
{
    ui->setupUi(this);

    connect(ui->cancel_Button, &QPushButton::clicked, this, &SingupView::goToLogin);
}

SingupView::~SingupView()
{
    delete ui;
}
