#ifndef COINSEARCHLINEEDIT_H
#define COINSEARCHLINEEDIT_H

#include <QLineEdit>

class CoinSearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CoinSearchLineEdit(QWidget *parent = nullptr);

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

signals:
    void focusIn();
    void focusOut();
};

#endif // COINSEARCHLINEEDIT_H
