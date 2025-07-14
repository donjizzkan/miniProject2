#include "chattinglistmanager.h"

#include <QList>

#include "chattingwindow.h"


// 더미 데이터 밑에 std::as_const 일시적 읽기전용으로 변경 - devwooms
// 여기에 const를 쓰면 바꿀수가 없어서 - devwooms
QList<QString> connectDUMMY = {"김수환무", "거북이", "두루미", "삼천갑자", "동방삭"};
QList<QString> oneByOneDUMMY = {"김씨", "심씨", "우씨", "조씨", "최씨"};
QList<QString> oneByMoreDUMMY = {"비트코인", "이더리움", "리플", "도지코인", "페페코인", "test"};



ChattingListManager::ChattingListManager(QObject *parent)
    : QObject{parent}
{




}

// ==============================
// ToolBox - devwooms
// ==============================

void ChattingListManager::settingChattingToolBox(QToolBox *chatting_ToolBox){
    chattingToolBox = chatting_ToolBox;
}

// ==============================
// 접속자 - devwooms
// ==============================

// 접속자 목록 세팅 - devwooms
void ChattingListManager::settingConnectListWidget(QListWidget *connect_ListWidget){
    connectListWidget = connect_ListWidget;
    updateConnectListWidget();

    // 더블 클릭 세팅 - devwooms
    connect(connect_ListWidget, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item){
        doubleClickActionConnectListWidget(item);
    });
}

// 리스트 업데이트 - devwooms
void ChattingListManager::updateConnectListWidget(){
    connectListWidget->clear();
    for(const auto& connectDUMMY : std::as_const(connectDUMMY)){
        connectListWidget -> addItem(connectDUMMY);
    }
}

// 더블 클릭 액션 - devwomms
void ChattingListManager::doubleClickActionConnectListWidget(QListWidgetItem *item){
    QString name = item->text();
    qDebug() << "connect_ListWidget name : " << name;
    // 중복 체크 - devwooms
    if (!oneByOneDUMMY.contains(name)) {
        oneByOneDUMMY.append(name);
        updateOneByOneListWidget();
        // 개인톡 목록으로 이동 - devwooms
        chattingToolBox->setCurrentIndex(1);
        // 채팅 윈도우 - devwooms
        ChattingWindow *chattingWindow = new ChattingWindow(name);
        chattingWindow->show();
    } else {
        qDebug() << "이미 존재하는 사용자입니다:" << name;
    }
}

// ==============================
// 개인톡 - devwooms
// ==============================

// 개인톡 세팅 - devwooms
void ChattingListManager::settingOneByOneListWidget(QListWidget *oneByone_listWidget){
    oneByonelistWidget = oneByone_listWidget;
    updateOneByOneListWidget();

    // 더블 클릭 세팅 - devwooms
    connect(oneByone_listWidget, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item){
        doubleClickActionOneByOneListWidget(item);
    });
}

// 리스트 업데이트 - devwooms
void ChattingListManager::updateOneByOneListWidget(){
    oneByonelistWidget->clear();
    for(const auto& oneByOneDUMMY : std::as_const(oneByOneDUMMY)){
        oneByonelistWidget -> addItem(oneByOneDUMMY);
    }
}

// 더블 클릭 액션 - devwomms
void ChattingListManager::doubleClickActionOneByOneListWidget(QListWidgetItem *item){
    QString name = item->text();
    qDebug() << "oneByone_listWidget name : " << name;
    // 채팅 윈도우 - devwooms
    ChattingWindow *chattingWindow = new ChattingWindow(name);
    chattingWindow->show();
}

// ==============================
// 단체톡 - devwooms
// ==============================

// 단체톡 세팅 - devwooms
void ChattingListManager::settingOneByMoreListWidget(QListWidget *oneByMore_listWidget){
    oneByMorelistWidget = oneByMore_listWidget;
    updateOneByMoreListWidget();

    // 더블 클릭 세팅 - devwooms
    connect(oneByMore_listWidget, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item){
        doubleClickActionOneByMoreListWidget(item);
    });
}

// 리스트 업데이트 - devwooms
void ChattingListManager::updateOneByMoreListWidget(){
    oneByMorelistWidget->clear();
    for(const auto& oneByMoreDUMMY : std::as_const(oneByMoreDUMMY)){
        oneByMorelistWidget -> addItem(oneByMoreDUMMY);
    }
}

// 더블 클릭 액션 - devwomms
void ChattingListManager::doubleClickActionOneByMoreListWidget(QListWidgetItem *item){
    QString name = item->text();
    qDebug() << "oneByMore_listWidget name : " << name;

    // 채팅 윈도우 - devwooms
    ChattingWindow *chattingWindow = new ChattingWindow(name);
    chattingWindow->show();
}
