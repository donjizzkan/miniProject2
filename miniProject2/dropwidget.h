#ifndef DROPWIDGET_H
#define DROPWIDGET_H

#include <QWidget>

class DropWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DropWidget(QWidget *parent = nullptr);

signals:
    void fileDropped(const QStringList &filePaths);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

#endif // DROPWIDGET_H
