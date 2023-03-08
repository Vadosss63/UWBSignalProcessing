QT += testlib
QT -= gui

include(../../include.pri)

unix:LIBS += -ltbb
unix:LIBS += -lboost_system
unix:LIBS += -lboost_thread
unix:LIBS += -lfftw3

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    test_avtoelectronicamodule.cpp \
    ../avtoelectronicablock/avtoelectronicamodule.cpp \
    ../../../DSPMLib/Tools/target.cpp \
    test_target.cpp

HEADERS += \
    ../avtoelectronicablock/avtoelectronicamodule.h \
    ../../../DSPMLib/Tools/target.h
