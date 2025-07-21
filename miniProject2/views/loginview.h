#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QPixmap>

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

    // 사용자 입력 데이터 접근자
    QString getInsertedID() const;
    QString getInsertedPW() const;

private slots:
    void handleLoginResponse(const QJsonObject &obj);

private:
    // UI 컴포넌트
    QLineEdit *id_LineEdit;
    QLineEdit *pw_LineEdit;
    QPushButton *signup_Button;
    QPushButton *login_Button;
    
    // 초기화 메서드들
    void setupUI();
    void setupSocketConnection();
    void connectSignals();
    
    // UI 생성 헬퍼 메서드들
    QHBoxLayout* createTitleLayout();
    QHBoxLayout* createLoginFormLayout();
    QHBoxLayout* createButtonLayout();
    
    // 유틸리티 메서드들
    void clearInputFields();

signals:
    void goToSignup();
    void goToMain();
    void doSignIn();
};

#endif // LOGINVIEW_H
