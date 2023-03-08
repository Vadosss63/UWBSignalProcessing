TARGET = AvtoelectronicaBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    avtoelectronicablock.cpp \
    avtoelectronicadialogbox.cpp \
    avtoelectronicamodule.cpp \
    ../../../DSPMLib/Tools/target.cpp

HEADERS += \
    avtoelectronicablock.h \
    avtoelectronicadialogbox.h \
    avtoelectronicamodule.h \
    ../../../DSPMLib/Tools/target.h

DISTFILES += avtoelectronicablock.json
