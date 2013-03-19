#-------------------------------------------------
#
# Project created by QtCreator 2013-01-17T17:37:33
#
#-------------------------------------------------

QT       += core gui sql network widgets

TARGET = To-Do-List-Manager
VERSION = 0.3
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    insertitem.cpp \
    edittask.cpp \
    preferences.cpp

HEADERS  += mainwindow.h \
    insertitem.h \
    edittask.h \
    preferences.h

FORMS    += mainwindow.ui \
    insertitem.ui \
    edittask.ui \
    preferences.ui
