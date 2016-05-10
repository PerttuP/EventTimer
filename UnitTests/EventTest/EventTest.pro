#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T13:22:01
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_eventtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


INCLUDEPATH +=  ../../EventTimer/inc/

DEPENDPATH += \
    ../../EventTimer/src/ \
    ../../EventTimer/inc/

SOURCES += \
    tst_eventtest.cc \
    ../../EventTimer/src/event.cc

DEFINES += SRCDIR=\\\"$$PWD/\\\"
