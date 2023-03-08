TARGET = TestFilterBlock
include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    testfilterblock.cpp \
    testfilterdialogbox.cpp \
    testfiltermodule.cpp \
    filterview.cpp \
    ../../../DSPMLib/Tools/signalprocessing.cpp

HEADERS += \
    testfilterblock.h \
    testfilterdialogbox.h \
    testfiltermodule.h \
    filterview.h \
    ../../../DSPMLib/Tools/signalprocessing.h


DISTFILES += testfilterblock.json

FORMS += \
    testfilterdialogbox.ui
