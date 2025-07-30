#include "coinsearchlineedit.h"

CoinSearchLineEdit::CoinSearchLineEdit(QWidget *parent)
    : QLineEdit{parent}
{


}
// FocusLineEdit.cpp
void CoinSearchLineEdit::focusInEvent(QFocusEvent *event) {
    QLineEdit::focusInEvent(event);
    emit focusIn();
}

void CoinSearchLineEdit::focusOutEvent(QFocusEvent *event) {
    QLineEdit::focusOutEvent(event);
    emit focusOut();
}
