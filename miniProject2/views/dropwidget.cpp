#include "dropwidget.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>


DropWidget::DropWidget(QWidget *parent, const QString& name)
    : QWidget{parent}, m_name(name)
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

    qDebug() << "드롭된 파일들:" << filePaths;
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "파일 전송",
        QString("선택된 파일(%1개)을 전송하시겠습니까?").arg(filePaths.size()),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        // Yes 선택 시 파일 전송 시그널 발생
        qDebug() << "파일 전송 승인";
        emit fileDropped(filePaths);
    } else {
        // No 선택 시
        qDebug() << "파일 전송 취소";
    }
}



