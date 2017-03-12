#-------------------------------------------------
#
# Project created by QtCreator 2016-02-21T20:22:22
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = soundrecord
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qsoundcard.cpp \
    waitfor.cpp

HEADERS  += mainwindow.h \
    wavefile.h \
    qsoundcard.h \
    waitfor.h

FORMS    += mainwindow.ui \
    waitfor.ui

DISTFILES +=

RESOURCES += \
    app.qrc
