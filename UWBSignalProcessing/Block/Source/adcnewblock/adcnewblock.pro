TARGET = ADCnewBlock

include(../../include.pri)

INCLUDEPATH += $${PATH_TO_API}/../UtilityUWB
DEPENDPATH += $${PATH_TO_API}/../UtilityUWB

DESTDIR = $$SOURCE_PATH

SOURCES += \
    adcnewblock.cpp \
    adcnewdialogbox.cpp \
    adcnewmodule.cpp \
    $${PATH_TO_API}/../UtilityUWB/errorcodeboost.cpp \
    $${PATH_TO_API}/../UtilityUWB/networkuwb.cpp \
    $${PATH_TO_API}/../UtilityUWB/boostlog.cpp \
    adccontroller.cpp

HEADERS += \
    adcnewblock.h \
    adcnewdialogbox.h \
    adcnewmodule.h \
    $${PATH_TO_API}/../UtilityUWB/errorcodeboost.h \
    $${PATH_TO_API}/../UtilityUWB/networkuwb.h \
    $${PATH_TO_API}/../UtilityUWB/boostlog.h \
    adccontroller.h \
    adcprotocol.h \
    connectprotocol.h

DISTFILES += adcnewblock.json
