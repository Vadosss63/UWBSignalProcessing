TARGET = AccumulationBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    accumulationblock.cpp \
    accumulationdialogbox.cpp \
    accumulationmodule.cpp

HEADERS += \
    accumulationblock.h \
    accumulationdialogbox.h \
    accumulationmodule.h

DISTFILES += accumulationblock.json
