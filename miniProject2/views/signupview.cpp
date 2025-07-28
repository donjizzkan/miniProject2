#include "signupview.h"
#include "models/sendingManage.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>

#include <QRandomGenerator>
#include <QTcpSocket>
#include <QTimer>
#include <QOverload>
#include <QAbstractSocket>
#include <QSslSocket>

SignupView::SignupView(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    // 창 제목 설정
    setWindowTitle("회원가입");

    // 입력 필드 초기화 및 설정
    setupInputFields();

    // 시그널-슬롯 연결
    connectSignals();

}

SignupView::~SignupView(){}

void SignupView::setupInputFields()
{
    // 비밀번호 필드를 패스워드 모드로 설정
    pw_LineEdit->setEchoMode(QLineEdit::Password);

    // 전화번호 입력 형식 제한 (숫자와 하이픈만 허용)
    QRegularExpression phoneRegex("^[0-9-]*$");
    QRegularExpressionValidator *phoneValidator = new QRegularExpressionValidator(phoneRegex, this);
    phoneNUM_LineEdit->setValidator(phoneValidator);

    // 플레이스홀더 텍스트 설정
    name_LineEdit->setPlaceholderText("이름을 입력하세요");
    id_LineEdit->setPlaceholderText("아이디를 입력하세요");
    pw_LineEdit->setPlaceholderText("비밀번호를 입력하세요");
    phoneNUM_LineEdit->setPlaceholderText("01012345678");
    email_LineEdit->setPlaceholderText("test@gmail.com");
    emailCheck_Lable->hide();
    emailCheck_LineEdit->hide();
    emailCheckNUM->hide();


    // 중복확인 라벨 초기 설정
    label_2->setText("중복확인");
    label_2->setStyleSheet("color: gray;");

}


//==========================
//      회원가입창 읽어옴
//==========================
userInfo SignupView::getUserInfo(){
    userInfo user;

    // lineEdit에 입력된 내용 읽어옴
    user.name = name_LineEdit->text();
    user.ID = id_LineEdit->text();
    user.phoneNum = phoneNUM_LineEdit->text();
    user.PW = pw_LineEdit->text();

    // 회원정보 초깃값 설정
    user.coinCnt = 0;               // 코인 갯수 = 0
    user.payment = 0;               // 총 지출액 = 0
    user.money = 10000000;          // 시작 금액 = 천만 원

    return user;
}

void SignupView::checkEmail(){



    QString emailText = email_LineEdit->text();

    // 이메일 정규식
    QRegularExpression emailcheck(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    QRegularExpressionMatch match = emailcheck.match(emailText);

    // 정규식에 맞는지 확인
    if(match.hasMatch()){

        qDebug() << "test1";
        emailCheck_Lable->show();
        emailCheck_LineEdit->show();
        emailCheckNUM->show();

        email_LineEdit->setReadOnly(true);
        emailCheck->hide();

        sendingManage sending;
        sending.sendEmailCheck(emailText);

    } else {
        QMessageBox msgBox;
        msgBox.setIconPixmap(QPixmap(":/assets/assets/warning.png"));
        msgBox.setText("이메일 형식이 올바르지 않습니다.");
        msgBox.exec();
    }

}

void SignupView::checkEmailNum(){
    QString emailCheckCode = emailCheck_LineEdit->text();
    sendingManage sending;
    sending.sendCodeEmailCheck(emailCheckCode);
}

void SignupView::checkEmailSignal(){
    emailCheckBool = true;
    emailCheckNUM ->hide();
    qDebug() << "이메일 인증 완료 - 회원가입 가능";
}

void SignupView::onSignupButtonClicked(){
    // 이메일 인증이 완료되었는지 확인
    if (!emailCheckBool) {
        QMessageBox msgBox;
        msgBox.setIconPixmap(QPixmap(":/assets/assets/warning.png"));
        msgBox.setText("이메일 인증을 먼저 완료해주세요.");
        msgBox.exec();
        return;
    }
    
    // 모든 필드가 입력되었는지 확인
    if (name_LineEdit->text().isEmpty() || 
        id_LineEdit->text().isEmpty() || 
        pw_LineEdit->text().isEmpty() || 
        phoneNUM_LineEdit->text().isEmpty() || 
        email_LineEdit->text().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIconPixmap(QPixmap(":/assets/assets/warning.png"));
        msgBox.setText("모든 필드를 입력해주세요.");
        msgBox.exec();
        return;
    }
    
    // 이메일 인증이 완료되었으므로 회원가입 진행
    emit doSignUp();
    qDebug() << "회원가입 진행";
}

// 시그널 연결
void SignupView::connectSignals()
{
    // 버튼 클릭 시그널 연결
    connect(emailCheck, &QPushButton::clicked, this, &SignupView::checkEmail);
    connect(emailCheckNUM, &QPushButton::clicked, this, &SignupView::checkEmailNum);
    connect(signup_Button, &QPushButton::clicked, this, &SignupView::onSignupButtonClicked);
    connect(cancel_Button, &QPushButton::clicked, this, &SignupView::goToLogin);
    // 이메일 확인 시그널
    connect(&SocketManage::instance(), &SocketManage::emailCheckResponseReceived, this, &SignupView::checkEmailSignal);
}

// Ui 설정
void SignupView::setupUI()
{
    // 메인 레이아웃
    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout(this);
    
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    
    // 상단 여백
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacer);
    
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    
    // 그리드 레이아웃
    QGridLayout *gridLayout = new QGridLayout();
    
    // 이름
    QLabel *name_Label = new QLabel("이름 : ");
    gridLayout->addWidget(name_Label, 0, 0);
    
    name_LineEdit = new QLineEdit();
    gridLayout->addWidget(name_LineEdit, 0, 1, 1, 2);
    
    // 아이디
    QLabel *id_Label = new QLabel("아이디 :");
    gridLayout->addWidget(id_Label, 1, 0);
    
    id_LineEdit = new QLineEdit();
    gridLayout->addWidget(id_LineEdit, 1, 1, 1, 2);
    
    label_2 = new QLabel("중복확인");
    gridLayout->addWidget(label_2, 2, 2);
    
    // 비밀번호
    QLabel *pw_Label = new QLabel("비밀번호 :");
    gridLayout->addWidget(pw_Label, 3, 0, 1, 2);
    
    pw_LineEdit = new QLineEdit();
    gridLayout->addWidget(pw_LineEdit, 3, 2);
    
    // 전화번호
    QLabel *phoneNUM_Label = new QLabel("전화번호 :");
    gridLayout->addWidget(phoneNUM_Label, 4, 0, 1, 2);
    
    phoneNUM_LineEdit = new QLineEdit();
    gridLayout->addWidget(phoneNUM_LineEdit, 4, 2);

    // 이메일
    QLabel *email_Label = new QLabel("이메일 :");
    gridLayout->addWidget(email_Label, 5, 0, 1, 2);

    email_LineEdit = new QLineEdit();
    gridLayout->addWidget(email_LineEdit, 5, 2);

    emailCheck = new QPushButton("이메일 인증");
    gridLayout->addWidget(emailCheck, 6, 2);

    // 인증 번호
    // emailCheck_Lable = new QLabel("05 : 00");
    emailCheck_Lable = new QLabel("");
    gridLayout->addWidget(emailCheck_Lable, 7, 0, 1, 2);

    emailCheck_LineEdit = new QLineEdit();
    gridLayout->addWidget(emailCheck_LineEdit, 7, 2);
    
    emailCheckNUM = new QPushButton("인증 확인");
    gridLayout->addWidget(emailCheckNUM, 8, 2);
    
    // 버튼들
    cancel_Button = new QPushButton("취소");
    gridLayout->addWidget(cancel_Button, 9, 0, 1, 2);
    
    signup_Button = new QPushButton("가입하기");
    gridLayout->addWidget(signup_Button, 9, 2);
    
    horizontalLayout->addLayout(gridLayout);
    
    QSpacerItem *horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer_2);
    
    verticalLayout->addLayout(horizontalLayout);
    
    // 하단 여백
    QSpacerItem *verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacer_2);
    
    horizontalLayout_2->addLayout(verticalLayout);
}
