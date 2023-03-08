TARGET = ExpBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    expblock.cpp \
    expdialogbox.cpp \
    expmodule.cpp

HEADERS += \
    expblock.h \
    expdialogbox.h \
    expmodule.h

DISTFILES += expblock.json
