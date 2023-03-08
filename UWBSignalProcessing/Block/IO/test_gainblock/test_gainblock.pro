QT += testlib
QT -= gui

include(../../include.pri)

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

unix:LIBS += -ltbb

INCLUDEPATH += $$PWD/../gainblock
DEPENDPATH += $$PWD/../gainblock

SOURCES += \
    test_gainmodule.cpp \
    ../gainblock/gainmodule.cpp


HEADERS += ../gainblock/gainmodule.h
