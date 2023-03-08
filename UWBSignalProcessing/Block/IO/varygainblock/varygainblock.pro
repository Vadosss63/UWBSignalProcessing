TARGET = VaryGainBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    varygainblock.cpp \
    varygaindialogbox.cpp \
    varygainmodule.cpp

HEADERS += \
    varygainblock.h \
    varygaindialogbox.h \
    varygainmodule.h


DISTFILES += varygainblock.json
