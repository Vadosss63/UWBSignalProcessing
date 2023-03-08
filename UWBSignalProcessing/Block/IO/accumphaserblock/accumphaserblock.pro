TARGET = AccumPhaserBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    accumphaserblock.cpp \
    accumphaserdialogbox.cpp \
    accumphasermodule.cpp

HEADERS += \
    accumphaserblock.h \
    accumphaserdialogbox.h \
    accumphasermodule.h


DISTFILES += accumphaserblock.json
