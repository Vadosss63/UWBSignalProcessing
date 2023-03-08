TARGET = ArithmeticBlock
include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    arithmeticblock.cpp \
    arithmeticdialogbox.cpp \
    arithmeticmodule.cpp \
    arithmeticstate.cpp

HEADERS += \
    arithmeticblock.h \
    arithmeticdialogbox.h \
    arithmeticmodule.h \
    arithmeticstate.h

DISTFILES += arithmeticblock.json
