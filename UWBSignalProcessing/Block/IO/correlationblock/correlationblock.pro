TARGET = CorrelationBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    correlationblock.cpp \
    correlationmodule.cpp

HEADERS += \
    correlationblock.h \
    correlationmodule.h


DISTFILES += correlationblock.json
