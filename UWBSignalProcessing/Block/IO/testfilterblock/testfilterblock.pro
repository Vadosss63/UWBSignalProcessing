TARGET = TestFilterBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

INCLUDEPATH += $${PATH_TO_API}/DSPMLib/Tools
DEPENDPATH += $${PATH_TO_API}/DSPMLib/Tools

SOURCES += \
    testfilterblock.cpp \
    testfilterdialogbox.cpp \
    testfiltermodule.cpp \
    filterview.cpp \
    $${PATH_TO_API}/DSPMLib/Tools/signalprocessing.cpp

HEADERS += \
    testfilterblock.h \
    testfilterdialogbox.h \
    testfiltermodule.h \
    filterview.h \
    $${PATH_TO_API}/DSPMLib/Tools/signalprocessing.h

DISTFILES += testfilterblock.json

FORMS += \
    testfilterdialogbox.ui
