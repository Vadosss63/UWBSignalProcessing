TARGET = ReadBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH

SOURCES += \
    readblock.cpp \
    readdialogbox.cpp \
    readmodule.cpp

HEADERS += \
    readblock.h \
    readdialogbox.h \
    readmodule.h

DISTFILES += readblock.json
