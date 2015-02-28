#-------------------------------------------------
#
# Project created by QtCreator 2015-02-28T12:14:08
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialBluetoothATCommands
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    arduinoprocess.cpp

HEADERS  += mainwindow.h \
    arduinoprocess.h

FORMS    += mainwindow.ui \
    arduinoprocess.ui

RESOURCES +=

