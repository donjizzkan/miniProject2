#ifndef HOMEVIEW_H
#define HOMEVIEW_H

#include <QWidget>

class ChartsToolBox; // forward declaration

namespace Ui {
class HomeView;
}

class HomeView : public QWidget
{
    Q_OBJECT

public:
    explicit HomeView(QWidget *parent = nullptr);
    ~HomeView();

private:
    Ui::HomeView *ui;
    ChartsToolBox *chartBox; // ChartsToolBox 인스턴스 - devwooms
};

#endif // HOMEVIEW_H
