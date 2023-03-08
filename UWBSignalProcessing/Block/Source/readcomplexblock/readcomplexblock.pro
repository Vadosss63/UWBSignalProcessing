TARGET = ReadComplexBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH

SOURCES += \
    readcomplexblock.cpp \
    readcomplexdialogbox.cpp \
    readcomplexmodule.cpp

HEADERS += \
    readcomplexblock.h \
    readcomplexdialogbox.h \
    readcomplexmodule.h


DISTFILES += readcomplexblock.json
