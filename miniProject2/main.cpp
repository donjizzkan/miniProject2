#include "mainwindow.h"
#include "splashview.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    // 스플레쉬 뷰
    SplashView *splash = new SplashView;
    splash -> show();

    // 2초 후 전환
    QTimer::singleShot(2000, [&]() {
        splash -> close();
        w.show();
    });

    return a.exec();
}
