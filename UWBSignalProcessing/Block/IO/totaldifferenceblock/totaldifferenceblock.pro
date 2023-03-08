
TARGET = TotalDifferenceblock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    totaldifferenceblock.cpp \
    totaldifferencemodule.cpp

HEADERS += \
    totaldifferenceblock.h \
    totaldifferencemodule.h

DISTFILES += totaldifferenceblock.json
