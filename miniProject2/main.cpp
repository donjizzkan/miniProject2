#include "mainwindow.h"
#include "splashview.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "miniProject2_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
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
