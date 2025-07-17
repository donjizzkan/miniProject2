#include "mainwindow.h"
#include "servermanager.h"
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

    // ==============================
    // 트레이 구성
    // ==============================

    // 현재 운영체제가 시스템 트레이 기능을 지원하는지 확인
    // 지원하지 않는다면 종료
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning("System tray is not available!");
        return 1;
    }

    // 트레이 아이콘 생성
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon(":/assets/icon.png"));

    // 트레이 메뉴 구
    QMenu *trayMenu = new QMenu();
    QAction *quitAction = new QAction("Quit");
    trayMenu->addAction(quitAction);

    // 액션에 해당하는 기능 연결
    QObject::connect(quitAction, &QAction::triggered, &a, &QApplication::quit);

    // 아이콘에 우클릭 설정 (trayMenu로)
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();


    // ==============================
    // 번역
    // ==============================
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "miniProjcet2_Server_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }


    // ==============================
    // 서버(싱글턴 인스턴스 가져옴)
    // ==============================
    ServerManager& server = ServerManager::getInstance();




    // ============================================================
    // 메인 윈도우 ( 테스트 코드 나중에는 사용하지 않고 트레이로만 )
    // ============================================================

    MainWindow w;
    w.show();
    return a.exec();
}
