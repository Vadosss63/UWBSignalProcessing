TARGET = PhaserBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    phaserblock.cpp \
    phaserdialogbox.cpp \
    phasermodule.cpp

HEADERS += \
    phaserblock.h \
    phaserdialogbox.h \
    phasermodule.h

DISTFILES += phaserblock.json
