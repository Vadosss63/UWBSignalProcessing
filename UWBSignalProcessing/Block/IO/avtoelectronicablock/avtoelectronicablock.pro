TARGET = AvtoelectronicaBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

INCLUDEPATH += $${PATH_TO_API}/DSPMLib/Tools
DEPENDPATH += $${PATH_TO_API}/DSPMLib/Tools

SOURCES += \
    avtoelectronicablock.cpp \
    avtoelectronicadialogbox.cpp \
    avtoelectronicamodule.cpp \
    $${PATH_TO_API}/DSPMLib/Tools/target.cpp

HEADERS += \
    avtoelectronicablock.h \
    avtoelectronicadialogbox.h \
    avtoelectronicamodule.h \
    $${PATH_TO_API}/DSPMLib/Tools/target.h

DISTFILES += avtoelectronicablock.json
