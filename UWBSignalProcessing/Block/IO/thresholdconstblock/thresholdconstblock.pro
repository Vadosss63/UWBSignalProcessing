TARGET = ThresholdConstBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    thresholdconstblock.cpp \
    thresholdconstdialogbox.cpp \
    thresholdconstmodule.cpp

HEADERS += \
    thresholdconstblock.h \
    thresholdconstdialogbox.h \
    thresholdconstmodule.h

DISTFILES += thresholdconstblock.json
