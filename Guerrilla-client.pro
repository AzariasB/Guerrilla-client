#-------------------------------------------------
#
# Project created by QtCreator 2018-01-30T11:10:03
#
#-------------------------------------------------

QT  += core gui websockets
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Guerrilla-client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        MainWindow.cpp \
    battlefield.cpp \
    unit.cpp \
    action.cpp \
    tree.cpp

HEADERS += \
        MainWindow.hpp \
    battlefield.hpp \
    unit.hpp \
    action.hpp \
    coordinates.hpp \
    tree.hpp
