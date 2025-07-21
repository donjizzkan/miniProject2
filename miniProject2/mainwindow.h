#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QMenuBar>
#include <QStatusBar>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

// Forward declarations
class LoginView;
class SignupView;
class HomeView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showLoginView();
    void showSignupView();
    void showHomeView();
    void handleLogin();
    void handleSignup();

private:
    // UI Components
    QStackedWidget *mainStackedWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    
    // Views
    LoginView *loginView;
    SignupView *signupView;
    HomeView *homeView;
    
    // Setup methods
    void setupUI();
    void setupViews();
    void connectSignals();
};
#endif // MAINWINDOW_H
