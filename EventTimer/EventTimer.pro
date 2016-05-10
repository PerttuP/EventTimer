
QT += core xml sql
QT -= gui

CONFIG += c++11

TEMPLATE = lib
TARGET = eventtimer

INCLUDEPATH += \
    inc/ \
    src/

HEADERS += \
    inc/eventtimer.hh \
    inc/event.hh \
    inc/eventhandler.hh \
    inc/logger.hh

SOURCES += \
    src/event.cc

