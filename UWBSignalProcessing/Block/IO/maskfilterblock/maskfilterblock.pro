TARGET = MaskFilterBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    maskfilterblock.cpp \
    maskfilterdialogbox.cpp \
    maskfiltermodule.cpp

HEADERS += \
    maskfilterblock.h \
    maskfilterdialogbox.h \
    maskfiltermodule.h

DISTFILES += maskfilterblock.json
