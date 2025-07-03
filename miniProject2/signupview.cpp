#include "signupview.h"
#include "ui_signupview.h"

SignupView::SignupView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignupView)
{
    ui->setupUi(this);

    connect(ui->cancel_Button, &QPushButton::clicked, this, &SignupView::goToLogin);
    connect(ui->signup_Button, &QPushButton::clicked, this, &SignupView::doSignUp);
}

SignupView::~SignupView()
{
    delete ui;
}

//==========================
//      회원가입창 읽어옴
//==========================
userInfo SignupView::getUserInfo(){
    userInfo user;

    // lineEdit에 입력된 내용 읽어옴
    user.name = ui->name_LineEdit->text();
    user.ID = ui->id_LineEdit->text();
    user.phoneNum = ui->phoneNUM_LineEdit->text();
    user.PW = ui->pw_LineEdit->text();

    // 회원정보 초깃값 설정
    user.coinCnt = 0;               // 코인 갯수 = 0
    user.payment = 0;               // 총 지출액 = 0
    user.money = 10000000;          // 시작 금액 = 천만 원

    return user;
}
