QT       += core gui charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chartstoolbox.cpp \
    chattinglistmanager.cpp \
    chattingwindow.cpp \
    dropwidget.cpp \
    loginview.cpp \
    main.cpp \
    mainview.cpp \
    mainwindow.cpp \
    sendingManage.cpp \
    signupview.cpp \
    userManage.cpp \
    splashview.cpp

HEADERS += \
    chartstoolbox.h \
    chattinglistmanager.h \
    chattingwindow.h \
    dropwidget.h \
    loginview.h \
    mainview.h \
    mainwindow.h \
    sendingManage.h \
    signupview.h \
    socketManage.h \
    userManage.h \
    splashview.h

FORMS += \
    chattingwindow.ui \
    loginview.ui \
    mainview.ui \
    mainwindow.ui \
    signupview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
