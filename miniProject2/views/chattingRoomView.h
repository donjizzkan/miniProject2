#ifndef CHATTINGROOMVIEW_H
#define CHATTINGROOMVIEW_H

#include "dropwidget.h"

#include <QWidget>
#include <QDebug>
#include <QJsonDocument>    // Json관련 라이브러리
#include <QJsonArray>       // Json관련 라이브러리
#include <QJsonObject>      // Json관련 라이브러리
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QPushButton>
#include <QLineEdit>
#include "models/socketManage.h"
#include "models/sendingManage.h"

class ChattingRoomView : public DropWidget
{
    Q_OBJECT

public:
    explicit ChattingRoomView(const QString& name, QWidget *parent = nullptr);
    ~ChattingRoomView();
    QByteArray buffer;

private:
    QString chatViewName;           // QWidget에서는 name을 멤버 함수로 가지고 있어서 코드에서는 안전하게 chatName으로 변환하는게 권고됨
    QTextBrowser *textBrowser;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QPushButton *pushButton_2;
    
    void setupUI();
};

#endif // CHATTINGROOMVIEW_H
