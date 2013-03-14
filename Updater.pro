#-------------------------------------------------
#
# Project created by QtCreator 2013-03-13T10:35:54
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = Updater
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    http.cpp \
    updatedata.cpp

HEADERS += http.h \
    updatedata.h

OTHER_FILES += \
    update.json
