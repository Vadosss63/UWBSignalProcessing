TARGET = SMABlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    smablock.cpp \
    smadialogbox.cpp \
    smamodule.cpp

HEADERS += \
    smablock.h \
    smadialogbox.h \
    smamodule.h


DISTFILES += smablock.json
