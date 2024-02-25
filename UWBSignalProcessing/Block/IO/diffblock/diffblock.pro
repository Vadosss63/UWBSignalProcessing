TARGET = DiffBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    diffblock.cpp \
    diffdialogbox.cpp \
    diffmodule.cpp

HEADERS += \
    diffblock.h \
    diffdialogbox.h \
    diffmodule.h

DISTFILES += diffblock.json
