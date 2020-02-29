#-------------------------------------------------
#
# Project created by QtCreator 2014-06-24T11:21:03
#
#-------------------------------------------------

QT       += core gui sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThreadDownload
TEMPLATE = app


SOURCES += main.cpp\
    downloadthread.cpp \
    downloadcontrol.cpp \
    widget.cpp \
    downloadwidgets.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    registwindow.cpp \
    taskwindow.cpp \
    updatewindow.cpp

HEADERS  += \
    downloadthread.h \
    downloadcontrol.h \
    config.h \
    widget.h \
    downloadwidgets.h \
    loginwindow.h \
    mainwindow.h \
    registwindow.h \
    taskwindow.h \
    updatewindow.h

FORMS    += mainwindow.ui \
    loginwindow.ui \
    registwindow.ui \
    taskwindow.ui \
    updatewindow.ui

RC_ICONS += download.ico
