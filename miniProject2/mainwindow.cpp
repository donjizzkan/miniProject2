#include "mainwindow.h"
#include "views/homeview.h"
#include "views/loginview.h"
#include "views/signupview.h"
#include "models/sendingManage.h"
#include "models/userManage.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupViews();
    connectSignals();
}

void MainWindow::setupViews()
{
    loginView = new LoginView(this);
    signupView = new SignupView(this);
    homeView = new HomeView(this);

    mainStackedWidget->addWidget(loginView);  // index 0
    mainStackedWidget->addWidget(signupView); // index 1
    mainStackedWidget->addWidget(homeView);   // index 2

    // 로그인 화면부터 시작
    mainStackedWidget->setCurrentIndex(0);
}

void MainWindow::connectSignals()
{

    // 로그인 화면 시그널 연결
    connect(loginView, &LoginView::goToSignup, this, &MainWindow::showSignupView);
    connect(loginView, &LoginView::goToMain, this, &MainWindow::showHomeView);
    connect(loginView, &LoginView::doSignIn, this, &MainWindow::handleLogin);

    // 회원가입 화면 시그널 연결
    connect(signupView, &SignupView::goToLogin, this, &MainWindow::showLoginView);
    connect(signupView, &SignupView::doSignUp, this, &MainWindow::handleSignup);
}

void MainWindow::setupUI()
{
    // 창 크기 설정
    resize(800, 548);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setWindowTitle("MainWindow");
    
    // 중앙 위젯
    QWidget *centralwidget = new QWidget(this);
    centralwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCentralWidget(centralwidget);
    
    // 레이아웃
    QHBoxLayout *horizontalLayout = new QHBoxLayout(centralwidget);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    
    // 메인 스택 위젯
    mainStackedWidget = new QStackedWidget(centralwidget);
    mainStackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainStackedWidget->setCurrentIndex(-1);
    horizontalLayout->addWidget(mainStackedWidget);
    
    // 메뉴바
    menubar = new QMenuBar(this);
    menubar->setGeometry(QRect(0, 0, 800, 18));
    setMenuBar(menubar);
    
    // 상태바
    statusbar = new QStatusBar(this);
    setStatusBar(statusbar);
}

void MainWindow::showLoginView()
{
    mainStackedWidget->setCurrentWidget(loginView);
}

void MainWindow::showSignupView()
{
    qDebug() << "Switching to signup view";
    mainStackedWidget->setCurrentWidget(signupView);
}

void MainWindow::showHomeView()
{
    mainStackedWidget->setCurrentWidget(homeView);
}

void MainWindow::handleLogin()
{
    sendingManage sending;
    QString ID = loginView->getInsertedID();
    QString PW = loginView->getInsertedPW();
    
    sending.sendLogIn(ID, PW);
}

void MainWindow::handleSignup()
{
    sendingManage sending;
    userInfo info = signupView->getUserInfo();
    
    sending.sendSignUp(info.name, info.ID, info.PW, info.phoneNum);
    showLoginView(); // 회원가입 후 로그인 화면으로 이동
}

MainWindow::~MainWindow() 
{
}
