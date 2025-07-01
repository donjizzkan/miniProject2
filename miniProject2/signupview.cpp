#include "signupview.h"
#include "ui_signupview.h"

SingupView::SingupView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SingupView)
{
    ui->setupUi(this);

    connect(ui->cancel_Button, &QPushButton::clicked, this, &SingupView::goToLogin);
    connect(ui->signup_Button, &QPushButton::clicked, this, &SingupView::goToLogin);
}

SingupView::~SingupView()
{
    delete ui;
}
