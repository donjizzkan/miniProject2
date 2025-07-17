QT       += core gui charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    models/loginmodel.cpp \
    models/mainmodel.cpp \
    networks/socketnetwork.cpp \
    views/splashview.cpp \
    views/mainview.cpp \
    views/loginview.cpp \
    candlechart.cpp \
    candlechartdatamanager.cpp \
    chartstoolbox.cpp \
    chattinglistmanager.cpp \
    chattingwindow.cpp \
    dropwidget.cpp \
    linechart.cpp \
    linechartdatamanager.cpp \
    sendingManage.cpp \
    signupview.cpp \
    socketManage.cpp \
    userManage.cpp


HEADERS += \
    mainwindow.h \
    models/loginmodel.h \
    models/mainmodel.h \
    views/splashview.h \
    views/mainview.h \
    views/loginview.h \
    candlechart.h \
    candlechartdatamanager.h \
    chartstoolbox.h \
    chattinglistmanager.h \
    chattingwindow.h \
    dropwidget.h \
    linechart.h \
    linechartdatamanager.h \
    sendingManage.h \
    signupview.h \
    socketManage.h \
    userManage.h


FORMS += \
    views/mainview.ui \
    views/mainwindow.ui \
    views/loginview.ui \
    views/signupview.ui \
    views/chattingwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
