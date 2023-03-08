TARGET = SourceGausBlock

include(../../include.pri)
DESTDIR = $$SOURCE_PATH

SOURCES += \
    sourcegausblock.cpp \
    sourcegausdialogbox.cpp \
    sourcegausmodule.cpp

HEADERS += \
    sourcegausblock.h \
    sourcegausdialogbox.h \
    sourcegausmodule.h

DISTFILES += sourcegausblock.json
