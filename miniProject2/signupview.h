#ifndef SIGNUPVIEW_H
#define SIGNUPVIEW_H

#include <QWidget>

namespace Ui {  
class SignupView;
}

class SignupView : public QWidget
{
    Q_OBJECT

public:
    explicit SignupView(QWidget *parent = nullptr);
    ~SignupView();

private:
    Ui::SignupView *ui;

// 메인윈도우에게 알릴 신호
signals:
    void goToLogin();
};

#endif // SIGNUPVIEW_H
