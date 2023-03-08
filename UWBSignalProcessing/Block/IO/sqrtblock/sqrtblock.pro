TARGET = SQRTBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    sqrtblock.cpp \
    sqrtdialogbox.cpp \
    sqrtmodule.cpp

HEADERS += \
    sqrtblock.h \
    sqrtdialogbox.h \
    sqrtmodule.h


DISTFILES += sqrtblock.json
