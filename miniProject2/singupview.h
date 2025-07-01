#ifndef SINGUPVIEW_H
#define SINGUPVIEW_H

#include <QWidget>

namespace Ui {
class SingupView;
}

class SingupView : public QWidget
{
    Q_OBJECT

public:
    explicit SingupView(QWidget *parent = nullptr);
    ~SingupView();

private:
    Ui::SingupView *ui;

// 메인윈도우에게 알릴 신호
signals:
    void goToLogin();
};

#endif // SINGUPVIEW_H
