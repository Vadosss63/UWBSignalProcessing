TARGET = ADCnewBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH
INCLUDEPATH += $$PWD/../../../../UtilityUWB
DEPENDPATH += $$PWD/../../../../UtilityUWB

SOURCES += \
    adcnewblock.cpp \
    adcnewdialogbox.cpp \
    adcnewmodule.cpp \
    ../../../../UtilityUWB/errorcodeboost.cpp \
    ../../../../UtilityUWB/networkuwb.cpp \
    ../../../../UtilityUWB/boostlog.cpp \
    adccontroller.cpp

HEADERS += \
    adcnewblock.h \
    adcnewdialogbox.h \
    adcnewmodule.h \
    ../../../../UtilityUWB/errorcodeboost.h \
    ../../../../UtilityUWB/networkuwb.h \
    ../../../../UtilityUWB/boostlog.h \
    adccontroller.h \
    adcprotocol.h \
    connectprotocol.h


DISTFILES += adcnewblock.json
