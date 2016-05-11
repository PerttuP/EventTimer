#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T23:59:06
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_databasehandlertest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../EventTimer/src \
    ../../EventTimer/inc \

DEPENDPATH += \
    ../../EventTimer/src

SOURCES += \
    tst_databasehandlertest.cc \
    ../../EventTimer/src/databasehandler.cc \
    ../../EventTimer/src/event.cc

DEFINES += SRCDIR=\\\"$$PWD/\\\"
