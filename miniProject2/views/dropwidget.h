#ifndef DROPWIDGET_H
#define DROPWIDGET_H

#include <QWidget>
#include <QMessageBox>

class DropWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DropWidget(QWidget *parent = nullptr, const QString& name = "");

private:
    QMessageBox::StandardButton reply;
    const QString& m_name;

signals:
    void fileDropped(const QStringList &filePaths);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

#endif // DROPWIDGET_H
