TARGET = BinBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    binblock.cpp \
    bindialogbox.cpp \
    binmodule.cpp

HEADERS += \
    binblock.h \
    bindialogbox.h \
    binmodule.h

DISTFILES += binblock.json
