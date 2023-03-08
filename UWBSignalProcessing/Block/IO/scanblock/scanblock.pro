TARGET = ScanBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    scanblock.cpp \
    scanmodule.cpp

HEADERS += \
    scanblock.h \
    scanmodule.h

DISTFILES += scanblock.json
