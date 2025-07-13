#ifndef CHATTINGWINDOW_H
#define CHATTINGWINDOW_H

#include "dropwidget.h"

#include <QWidget>
#include <QDebug>
#include <QJsonDocument>    // Json관련 라이브러리
#include <QJsonArray>       // Json관련 라이브러리
#include <QJsonObject>      // Json관련 라이브러리
#include "socketManage.h"
#include "sendingManage.h"

namespace Ui {
class ChattingWindow;
}

class ChattingWindow : public DropWidget
{
    Q_OBJECT

public:
    explicit ChattingWindow(const QString& name, QWidget *parent = nullptr);
    ~ChattingWindow();

private:
    QString chatViewName;           // QWidget에서는 name을 멤버 함수로 가지고 있어서 코드에서는 안전하게 chatName으로 변환하는게 권고됨
    Ui::ChattingWindow *ui;
};

#endif // CHATTINGWINDOW_H
