#include "loginview.h"
#include "models/sendingManage.h"
#include "models/socketManage.h"
#include <QDebug>
#include <QMessageBox>
#include <QIcon>
#include <QFont>
#include <QTcpSocket>

LoginView::LoginView(QWidget *parent) : QWidget(parent)
{
    setupUI();
    connectSignals();
    setupSocketConnection();
}

void LoginView::setupSocketConnection()
{
    // 소켓 연겴 초기화
    QTcpSocket *socket = SocketManage::instance().socket();
    
    if (socket->state() == QAbstractSocket::UnconnectedState) {
        // TODO: 서버 주소를 설정 파일에서 불러오도록 개선 필요
        socket->connectToHost("192.168.2.234", 51234);
    }
}


void LoginView::handleLoginResponse(const QJsonObject &obj)
{
    qDebug() << "LoginView: 로그인 응답 수신";
    
    if (obj.value("type").toString() != "response") {
        return;
    }
    
    QString result = obj.value("result").toString();
    
    if (result == "success") {
        handleLoginSuccess(obj.value("name").toString());
    } else {
        handleLoginFailure();
    }
}

void LoginView::handleLoginSuccess(const QString &senderName)
{
    qDebug() << senderName << "으로 로그인 성공";
    
    // 사용자 이름 저장
    sendingManage::instance()->setSenderName(senderName);
    
    // 메인 화면으로 이동
    emit goToMain();
}

void LoginView::handleLoginFailure()
{
    QMessageBox::warning(this, "로그인 실패", "ID 또는 PW가 틀렸습니다.");
    qDebug() << "로그인 실패: ID 또는 PW 불일치";
    
    // 입력 필드 초기화
    clearInputFields();
}

void LoginView::clearInputFields()
{
    pw_LineEdit->clear();
    pw_LineEdit->setFocus();
}

void LoginView::connectSignals()
{
    // 로그인 응답 처리
    connect(&SocketManage::instance(), &SocketManage::loginResponseReceived, this, &LoginView::handleLoginResponse);

    // UI 이벤트 연결
    connect(pw_LineEdit, &QLineEdit::returnPressed, this, [this]() { login_Button->click(); });
    connect(signup_Button, &QPushButton::clicked, this, &LoginView::goToSignup);
    connect(login_Button, &QPushButton::clicked, this, &LoginView::doSignIn);
}

void LoginView::setupUI()
{
    // 메인 레이아웃 설정
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QVBoxLayout *centerLayout = new QVBoxLayout();
    
    // 상단 여백
    centerLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    
    // 타이틀 영역 생성
    centerLayout->addLayout(createTitleLayout());
    
    // 로그인 폼 생성
    centerLayout->addLayout(createLoginFormLayout());
    
    // 하단 여백
    centerLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    
    mainLayout->addLayout(centerLayout);
}

QHBoxLayout* LoginView::createTitleLayout()
{
    QHBoxLayout *titleLayout = new QHBoxLayout();
    
    // 좌우 여백
    titleLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    
    // 타이틀 컴포넌트들
    QHBoxLayout *titleComponents = new QHBoxLayout();
    
    // "Down Bit" 레이블
    QLabel *titleLabel = new QLabel("Down Bit");
    QFont titleFont;
    titleFont.setPointSize(30);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleComponents->addWidget(titleLabel);
    
    // 비트코인 아이콘
    QLabel *iconLabel = new QLabel();
    iconLabel->setMaximumSize(100, 100);
    iconLabel->setPixmap(QPixmap(":/assets/assets/Bitcoin.png"));
    iconLabel->setScaledContents(true);
    titleComponents->addWidget(iconLabel);
    
    titleLayout->addLayout(titleComponents);
    titleLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    
    return titleLayout;
}

QHBoxLayout* LoginView::createLoginFormLayout()
{
    QHBoxLayout *formLayout = new QHBoxLayout();
    
    // 좌우 여백
    formLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    
    // 입력 필드들
    QVBoxLayout *inputLayout = new QVBoxLayout();
    
    // 폰트 설정
    QFont inputFont;
    inputFont.setPointSize(20);
    
    // ID 입력 필드
    id_LineEdit = new QLineEdit();
    id_LineEdit->setMinimumHeight(30);
    id_LineEdit->setFont(inputFont);
    id_LineEdit->setPlaceholderText("아이디를 입력하세요");
    inputLayout->addWidget(id_LineEdit);
    
    // 비밀번호 입력 필드
    pw_LineEdit = new QLineEdit();
    pw_LineEdit->setMinimumHeight(30);
    pw_LineEdit->setFont(inputFont);
    pw_LineEdit->setEchoMode(QLineEdit::Password);
    pw_LineEdit->setPlaceholderText("비밀번호를 입력하세요");
    inputLayout->addWidget(pw_LineEdit);
    
    // 버튼 레이아웃
    inputLayout->addLayout(createButtonLayout());
    
    formLayout->addLayout(inputLayout);
    formLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    
    return formLayout;
}

QHBoxLayout* LoginView::createButtonLayout()
{
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    // 버튼 폰트 설정
    QFont buttonFont;
    buttonFont.setPointSize(15);
    buttonFont.setBold(true);
    
    // 회원가입 버튼
    signup_Button = new QPushButton("회원가입");
    signup_Button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    signup_Button->setMinimumSize(200, 30);
    signup_Button->setMaximumSize(200, 30);
    signup_Button->setFont(buttonFont);
    buttonLayout->addWidget(signup_Button);
    
    // 로그인 버튼
    login_Button = new QPushButton("로그인");
    login_Button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    login_Button->setMinimumSize(200, 30);
    login_Button->setMaximumSize(200, 30);
    login_Button->setFont(buttonFont);
    login_Button->setDefault(true); // 기본 버튼으로 설정
    buttonLayout->addWidget(login_Button);
    
    return buttonLayout;
}

LoginView::~LoginView() { }

QString LoginView::getInsertedID() const
{
    return id_LineEdit->text().trimmed();
}

QString LoginView::getInsertedPW() const
{
    return pw_LineEdit->text();
}
