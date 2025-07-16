#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>

class ChartsToolBox; // forward declaration

namespace Ui {
class MainView;
}

class MainView : public QWidget
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = nullptr);
    ~MainView();

private:
    Ui::MainView *ui;
    ChartsToolBox *chartBox; // ChartsToolBox 인스턴스 - devwooms
};

#endif // MAINVIEW_H
