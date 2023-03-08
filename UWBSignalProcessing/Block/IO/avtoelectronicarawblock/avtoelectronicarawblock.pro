TARGET = AvtoelectronicaRawBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    avtoelectronicarawblock.cpp \
    avtoelectronicarawdialogbox.cpp \
    avtoelectronicarawmodule.cpp

HEADERS += \
    avtoelectronicarawblock.h \
    avtoelectronicarawmodule.h \
    avtoelectronicarawdialogbox.h

DISTFILES += avtoelectronicarawblock.json
