TARGET = ThresholdBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    thresholdblock.cpp \
    thresholdmodule.cpp

HEADERS += \
    thresholdblock.h \
    thresholdmodule.h

DISTFILES += thresholdblock.json
