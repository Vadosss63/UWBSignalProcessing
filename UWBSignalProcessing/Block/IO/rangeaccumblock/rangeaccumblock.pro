TARGET = RangeAccumBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    rangeaccumblock.cpp \
    rangeaccumdialogbox.cpp \
    rangeaccummodule.cpp \
    windows.cpp

HEADERS += \
    rangeaccumblock.h \
    rangeaccumdialogbox.h \
    rangeaccummodule.h \
    windows.h

DISTFILES += rangeaccumblock.json
