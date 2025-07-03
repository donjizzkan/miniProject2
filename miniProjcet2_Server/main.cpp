#include "mainwindow.h"

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QLocale>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning("System tray is not available!");
        return 1;
    }

    // 트레이 아이콘 생성
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon(":/assets/icon.png"));

    // 트레이 메뉴 구성
    QMenu *trayMenu = new QMenu();
    QAction *quitAction = new QAction("Quit");
    trayMenu->addAction(quitAction);

    QObject::connect(quitAction, &QAction::triggered, &a, &QApplication::quit);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "miniProjcet2_Server_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }






    MainWindow w;
    w.show();
    return a.exec();
}
