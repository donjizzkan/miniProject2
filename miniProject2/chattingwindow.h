#ifndef CHATTINGWINDOW_H
#define CHATTINGWINDOW_H

#include <QWidget>

namespace Ui {
class ChattingWindow;
}

class ChattingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChattingWindow(QWidget *parent = nullptr);
    ~ChattingWindow();

private:
    Ui::ChattingWindow *ui;
};

#endif // CHATTINGWINDOW_H
