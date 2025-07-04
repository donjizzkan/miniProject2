#include "dropwidget.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

DropWidget::DropWidget(QWidget *parent)
    : QWidget{parent}
{
    // 드랍을 허용 - devwooms
    setAcceptDrops(true);
}

// 드래그가 해당 위젯에 인식됬을때 - devwooms
void DropWidget::dragEnterEvent(QDragEnterEvent *event){
    // mimeData : 데이터의 종류와 실제 데이터 - devwooms
    // hasUrls : 해당 데이터의 경로 - devwooms
    // 파일인지 검사 후 - devwooms
    if (event->mimeData()->hasUrls()){
        // 드래그엔드랍 허용 - devwooms
        event->acceptProposedAction();
    }
}

// 드랍 이벤트 - devwooms
void DropWidget::dropEvent(QDropEvent *event){
    QStringList filePaths;
    // 파일 수 만큼 url을 추출 후 리스트로 넣고 - devwooms
    const QList<QUrl> urls = event->mimeData()->urls();
    for (const QUrl &url : urls) {
        filePaths << url.toLocalFile();
    }
    qDebug() << "test : " << filePaths;
    // 시그널로 전달 - devwooms
    emit fileDropped(filePaths);
}
