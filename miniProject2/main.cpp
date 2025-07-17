#include "mainwindow.h"
#include "views/splashview.h"
#include <views/homeview.h>

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTimer>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // 스플레쉬 뷰 - devwooms
    SplashView *splash = new SplashView;
    splash -> show();

    // 2초 후 전환 - devwooms
    QTimer::singleShot(2000, &w, [&]() {
        splash -> close();
        w.show();
    });

    return a.exec();
}
