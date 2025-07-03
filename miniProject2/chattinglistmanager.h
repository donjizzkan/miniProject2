#ifndef CHATTINGLISTMANAGER_H
#define CHATTINGLISTMANAGER_H

#include <QObject>
#include <QListWidget>
#include <QToolBox>

class ChattingListManager : public QObject
{
    Q_OBJECT
public:
    explicit ChattingListManager(QObject *parent = nullptr);

    void settingChattingToolBox(QToolBox *chatting_ToolBox);

    void settingConnectListWidget(QListWidget *connect_ListWidget);
    void settingOneByOneListWidget(QListWidget *oneByone_listWidget);
    void settingOneByMoreListWidget(QListWidget *oneByMore_listWidget);

    void updateConnectListWidget();
    void updateOneByOneListWidget();
    void updateOneByMoreListWidget();

    void doubleClickActionConnectListWidget(QListWidgetItem *item);
    void doubleClickActionOneByOneListWidget(QListWidgetItem *item);
    void doubleClickActionOneByMoreListWidget(QListWidgetItem *item);

private:
    QToolBox *chattingToolBox;
    QListWidget *connectListWidget;
    QListWidget *oneByonelistWidget;
    QListWidget *oneByMorelistWidget;



signals:
};

#endif // CHATTINGLISTMANAGER_H
