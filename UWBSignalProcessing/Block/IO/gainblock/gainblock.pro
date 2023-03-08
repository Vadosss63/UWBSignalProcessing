TARGET = GainBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    gainblock.cpp \
    gaindialogbox.cpp \
    gainmodule.cpp

HEADERS += \
    gainblock.h \
    gaindialogbox.h \
    gainmodule.h


DISTFILES += gainblock.json
