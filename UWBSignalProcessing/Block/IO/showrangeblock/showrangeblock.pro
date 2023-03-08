TARGET = ShowRangeBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    showrangeblock.cpp \
    showrangemodule.cpp \
    showrangedialogbox.cpp

HEADERS += \
    showrangeblock.h \
    showrangemodule.h \
    showrangedialogbox.h

DISTFILES += showrangeblock.json
