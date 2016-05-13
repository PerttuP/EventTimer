#-------------------------------------------------
#
# Project created by QtCreator 2016-05-13T15:47:50
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_databasehandlerbenchmark
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../EventTimer/src/ \
    ../../EventTimer/inc/

DEPENDPATH += \
    ../../EventTimer/src/


SOURCES += \
    tst_databasehandlerbenchmark.cc \
    ../../EventTimer/src/databasehandler.cc \
    ../../EventTimer/src/event.cc


DEFINES += SRCDIR=\\\"$$PWD/\\\"
