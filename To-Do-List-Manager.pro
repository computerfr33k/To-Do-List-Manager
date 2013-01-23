#-------------------------------------------------
#
# Project created by QtCreator 2013-01-17T17:37:33
#
#-------------------------------------------------

QT       += core gui sql

TARGET = To-Do-List-Manager
VERSION = 0.2
TEMPLATE = app

# Fervor autoupdater
# (set TARGET and VERSION of your app before including Fervor.pri)
!include("fervor/Fervor.pri") {
        error("Unable to include Fervor autoupdater.")
}

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
