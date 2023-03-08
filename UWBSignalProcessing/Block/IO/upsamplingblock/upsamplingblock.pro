TARGET = UpsamplingBlock
include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    upsamplingblock.cpp \
    upsamplingdialogbox.cpp \
    upsamplingmodule.cpp

HEADERS += \
    upsamplingblock.h \
    upsamplingdialogbox.h \
    upsamplingmodule.h

DISTFILES += upsamplingblock.json
