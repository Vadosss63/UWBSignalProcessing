TARGET = TransferFrequencyBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    transferfrequencyblock.cpp \
    transferfrequencydialogbox.cpp \
    transferfrequencymodule.cpp

HEADERS += \
    transferfrequencyblock.h \
    transferfrequencydialogbox.h \
    transferfrequencymodule.h

DISTFILES += transferfrequencyblock.json
