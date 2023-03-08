TARGET = VRLIScanBlock
include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    vrliscanblock.cpp \
    vrliscandialogbox.cpp \
    vrliscanmodule.cpp

HEADERS += \
    vrliscanblock.h \
    vrliscandialogbox.h \
    vrliscanmodule.h

DISTFILES += vrliscanblock.json
