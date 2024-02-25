TARGET = VARYBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    varyblock.cpp \
    varydialogbox.cpp \
    varymodule.cpp

HEADERS += \
    varyblock.h \
    varydialogbox.h \
    varymodule.h

DISTFILES += varyblock.json
