TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += \
        main.cpp \
    ../Utility/network.cpp

INCLUDEPATH += $$PWD/../Utility
LIBS += -lboost_system
LIBS += -pthread

HEADERS += \
    ../Utility/network.h
