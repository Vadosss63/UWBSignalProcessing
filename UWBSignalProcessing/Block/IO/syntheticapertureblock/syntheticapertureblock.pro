TARGET = SyntheticAapertureBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    syntheticapertureblock.cpp \
    syntheticaperturedialogbox.cpp \
    syntheticaperturemodule.cpp

HEADERS += \
    syntheticapertureblock.h \
    syntheticaperturedialogbox.h \
    syntheticaperturemodule.h

DISTFILES += syntheticapertureblock.json
