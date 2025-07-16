#include "mainwindow.h"
#include "splashview.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTimer>

#include <mainview.h>

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

    // MainView *testcode = new MainView();  // 중복 생성 제거 - devwooms
    // testcode->show();

    return a.exec();
}
