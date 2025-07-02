#include "signupview.h"
#include "ui_signupview.h"

SignupView::SignupView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignupView)
{
    ui->setupUi(this);

    connect(ui->cancel_Button, &QPushButton::clicked, this, &SignupView::goToLogin);
    connect(ui->signup_Button, &QPushButton::clicked, this, &SignupView::goToLogin);
}

SignupView::~SignupView()
{
    delete ui;
}
