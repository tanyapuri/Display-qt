

QT       += core gui network
QT       += multimedia
RC_ICONS += 1.ico
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gift
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
    initialize.cpp \
        mainwindow.cpp \
    juldate.cpp \
    calculate.cpp \
    memo_save.cpp \
    memo_c.cpp \
    mthread.cpp \
    qpositionwidget.cpp \
    qvolumecontrol.cpp \
    qvolumeview.cpp \
    tv.cpp \
    tv_input.cpp \
    popup.cpp \
    settings.cpp \
    code.cpp

HEADERS  += mainwindow.h \
    initialize.h \
    juldate.h \
    calculate.h \
    memo_save.h \
    memo_c.h \
    mthread.h \
    qpositionwidget.h \
    qvolumecontrol.h \
    qvolumeview.h \
    tv.h \
    tv_input.h \
    popup.h \
    settings.h \
    code.h

FORMS    += mainwindow.ui \
    calculate.ui \
    memo_save.ui \
    memo_c.ui \
    tv.ui \
    tv_input.ui \
    popup.ui \
    settings.ui \
    code.ui

RESOURCES += \
    image.qrc \
    res.qrc
