TARGET = TimeDelayBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    timedelayblock.cpp \
    timedelaydialogbox.cpp \
    timedelaymodule.cpp

HEADERS += \
    timedelayblock.h \
    timedelaydialogbox.h \
    timedelaymodule.h

DISTFILES += timedelayblock.json
