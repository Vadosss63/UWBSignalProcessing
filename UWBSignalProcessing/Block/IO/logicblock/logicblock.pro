TARGET = LogicBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    logicblock.cpp \
    logicmodule.cpp

HEADERS += \
    logicblock.h \
    logicmodule.h

DISTFILES += logicblock.json
