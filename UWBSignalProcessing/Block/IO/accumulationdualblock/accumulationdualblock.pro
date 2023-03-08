TARGET = AccumulationDualBlock

include(../../include.pri)

DESTDIR = $$IO_PATH


SOURCES += \
    accumulationdualblock.cpp \
    accumulationdualdialogbox.cpp \
    accumulationdualmodule.cpp

HEADERS += \
    accumulationdualblock.h \
    accumulationdualdialogbox.h \
    accumulationdualmodule.h

DISTFILES += accumulationdualblock.json
