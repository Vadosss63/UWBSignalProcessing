TARGET = MathBlock
include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    mathblock.cpp \
    mathdialogbox.cpp \
    mathmodule.cpp \
    mathstate.cpp

HEADERS += \
    mathblock.h \
    mathdialogbox.h \
    mathmodule.h \
    mathstate.h

DISTFILES += mathblock.json
