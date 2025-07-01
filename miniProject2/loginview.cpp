#include "loginview.h"
#include "ui_loginview.h"

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);

    connect(ui->signupButton, &QPushButton::clicked, this, &LoginView::goToSignup);

}

LoginView::~LoginView()
{
    delete ui;
}
