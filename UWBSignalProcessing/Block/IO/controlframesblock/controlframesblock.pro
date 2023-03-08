TARGET = ControlFramesBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    controlframesblock.cpp \
    controlframesmodule.cpp

HEADERS += \
    controlframesblock.h \
    controlframesmodule.h

DISTFILES += controlframesblock.json
