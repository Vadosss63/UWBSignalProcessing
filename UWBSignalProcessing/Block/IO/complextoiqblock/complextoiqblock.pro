
TARGET = ComplexToIQblock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    complextoiqblock.cpp \
    complextoiqmodule.cpp

HEADERS += \
    complextoiqblock.h \
    complextoiqmodule.h

DISTFILES += complextoiqblock.json
