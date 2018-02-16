#-------------------------------------------------
#
# Project created by QtCreator 2018-02-14T06:48:43
#
#-------------------------------------------------

QT       += websockets

QT       -= gui

INCLUDEPATH += ".."

TARGET = tst_MainTest
CONFIG   += console c++14
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
        tst_MainTest.cpp  \
        ..\battlefield.cpp \
        ..\unit.cpp \
        ..\action.cpp \
        ..\tree.cpp

HEADERS += \
    ..\battlefield.hpp \
    ..\unit.hpp \
    ..\action.hpp \
    ..\coordinates.hpp \
    ..\tree.hpp


DEFINES += SRCDIR=\\\"$$PWD/\\\"

RESOURCES += \
    config.qrc
