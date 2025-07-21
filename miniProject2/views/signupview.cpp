#include "signupview.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

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
    phoneNUM_LineEdit->setPlaceholderText("010-1234-5678");

    // 중복확인 라벨 초기 설정
    label_2->setText("중복확인");
    label_2->setStyleSheet("color: gray;");
    label_4->setText("중복확인");
    label_4->setStyleSheet("color: gray;");
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

// 시그널 연결
void SignupView::connectSignals()
{
    // 버튼 클릭 시그널 연결
    connect(signup_Button, &QPushButton::clicked, this, &SignupView::doSignUp);
    connect(cancel_Button, &QPushButton::clicked, this, &SignupView::goToLogin);
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
    
    label_4 = new QLabel("중복확인");
    gridLayout->addWidget(label_4, 5, 2);
    
    // 버튼들
    cancel_Button = new QPushButton("취소");
    gridLayout->addWidget(cancel_Button, 6, 0, 1, 2);
    
    signup_Button = new QPushButton("가입하기");
    gridLayout->addWidget(signup_Button, 6, 2);
    
    horizontalLayout->addLayout(gridLayout);
    
    QSpacerItem *horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer_2);
    
    verticalLayout->addLayout(horizontalLayout);
    
    // 하단 여백
    QSpacerItem *verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacer_2);
    
    horizontalLayout_2->addLayout(verticalLayout);
}
