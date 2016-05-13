#-------------------------------------------------
#
# Project created by QtCreator 2016-05-13T20:41:11
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_eventtimerlogictest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../EventTimer/src/ \
    ../../EventTimer/inc/

DEPENDPATH += \
    ../../EventTimer/src/ \
    ../../EventTimer/inc/

HEADERS += \
    ../../EventTimer/src/eventtimerlogic.hh

SOURCES += \
    tst_eventtimerlogictest.cc \
    ../../EventTimer/src/event.cc \
    ../../EventTimer/src/eventtimerlogic.cc \
    ../../EventTimer/src/databasehandler.cc \
    ../../EventTimer/src/eventtimerbuilder.cc


DEFINES += SRCDIR=\\\"$$PWD/\\\"
