#include "chattinglistmanager.h"

#include <QList>
#include <QLabel>
#include "views/chattingRoomView.h"


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
    updateData(connectListWidget, connectDUMMY);

    // 더블 클릭 세팅 - devwooms
    connect(connectListWidget, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item){
        doubleClickActionConnectListWidget(item);
    });
}

// 더블 클릭 액션 - devwomms
void ChattingListManager::doubleClickActionConnectListWidget(QListWidgetItem *item){
    QString name = item->text();
    qDebug() << "connect_ListWidget name : " << name;
    // 중복 체크 - devwooms
    if (!oneByOneDUMMY.contains(name)) {
        oneByOneDUMMY.append(name);
        updateData(oneByonelistWidget, oneByOneDUMMY);
        // 개인톡 목록으로 이동 - devwooms
        chattingToolBox->setCurrentIndex(1);
        // 채팅 윈도우 - devwooms
        ChattingRoomView *chattingRoomView = new ChattingRoomView(name);
        chattingRoomView->show();
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
    updateData(oneByonelistWidget, oneByOneDUMMY);
    dragAnddrop(oneByonelistWidget);


    // 더블 클릭 세팅 - devwooms
    connect(oneByonelistWidget, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item){
        doubleClickActionOneByOneListWidget(item);
    });
    // 드래그 앤 드랍 update 세팅
    connect(connectListWidget->model(), &QAbstractItemModel::rowsMoved, this, []() {
        qDebug() << "리스트 순서가 변경되었습니다!";
    });
}

// 더블 클릭 액션 - devwomms
void ChattingListManager::doubleClickActionOneByOneListWidget(QListWidgetItem *item){
    QString name = item->text();
    qDebug() << "oneByone_listWidget name : " << name;
    // 채팅 윈도우 - devwooms
    ChattingRoomView *chattingRoomView = new ChattingRoomView(name);
    chattingRoomView->show();
}

// ==============================
// 단체톡 - devwooms
// ==============================

// 단체톡 세팅 - devwooms
void ChattingListManager::settingOneByMoreListWidget(QListWidget *oneByMore_listWidget){
    oneByMorelistWidget = oneByMore_listWidget;
    updateData(oneByMorelistWidget, oneByMoreDUMMY);

    // 더블 클릭 세팅 - devwooms
    connect(oneByMorelistWidget, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item){
        doubleClickActionOneByMoreListWidget(item);
    });
}

// 더블 클릭 액션 - devwomms
void ChattingListManager::doubleClickActionOneByMoreListWidget(QListWidgetItem *item){
    QString name = item->text();
    qDebug() << "oneByMore_listWidget name : " << name;

    // 채팅 윈도우 - devwooms
    ChattingRoomView *chattingRoomView = new ChattingRoomView(name);
    chattingRoomView->show();
}

// ==============================
// 공통 함수 - devwooms
// ==============================

// 리스트 업데이트 - devwooms
void ChattingListManager::updateData(QListWidget *qListWidget, QList<QString> list){
    qListWidget->clear();
    for(const auto& list : std::as_const(list)){
        qListWidget -> addItem(list);
    }
}

// DragAndDrop - devwooms
void ChattingListManager::dragAnddrop(QListWidget *qListWidget){
    // 드래그 앤 드랍 세팅 -devwooms
    qListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    qListWidget->setDragEnabled(true);
    qListWidget->setAcceptDrops(true);
    qListWidget->setDropIndicatorShown(true);
    qListWidget->setDragDropMode(QAbstractItemView::InternalMove);
}

