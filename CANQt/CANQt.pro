QT       += core gui opengl widgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    objectgl.cpp \
    socketcan_cpp.cpp \
    waitingConnection_window.cpp

HEADERS += \
    mainwindow.h \
    objectgl.h \
    socketcan_cpp.h \
    socketcan_cpp_export.h \
    ui_waitingConnection_window.h \
    waitingConnection_window.h

FORMS += \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
