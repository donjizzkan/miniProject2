#include "splashview.h"

#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QVBoxLayout>

SplashView::SplashView(QWidget *parent)
    : QWidget{parent}
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    resize(400, 400);

    auto *layout = new QVBoxLayout(this);
    // Qt에서 텍스트나 이미지 또는 애니메이션을 표시할 수 있는 기본 위젯
    // 해서 애니메이션을 표현하기 위해서 QMovie의 화면으로씀
    QLabel *gifLabel = new QLabel(this);
    // 배경을 투명으로
    gifLabel->setAttribute(Qt::WA_TranslucentBackground);
    gifLabel->setStyleSheet("background: transparent;");

    // 리소스에 등록한 gif
    QMovie *movie = new QMovie(":/assets/assets/bitAnimation.gif");
    gifLabel->setMovie(movie);
    movie->start();

    layout->addWidget(gifLabel);
    layout->setAlignment(Qt::AlignCenter);

}
