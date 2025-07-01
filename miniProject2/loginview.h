#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>

namespace Ui {
class LoginView;
}

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

private:
    Ui::LoginView *ui;

// 메인윈도우에게 알릴 신호
signals:
    void goToSignup();
};

#endif // LOGINVIEW_H
