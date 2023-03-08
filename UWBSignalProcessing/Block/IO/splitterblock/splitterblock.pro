
TARGET = SplitterBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    splitterblock.cpp \
    splitterdialogbox.cpp \
    splittermodule.cpp

HEADERS += \
    splitterblock.h \
    splitterdialogbox.h \
    splittermodule.h


DISTFILES += splitterblock.json
