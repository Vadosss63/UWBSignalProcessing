TARGET = TwoGainBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    twogainblock.cpp \
    twogaindialogbox.cpp \
    twogainmodule.cpp

HEADERS += \
    twogainblock.h \
    twogaindialogbox.h \
    twogainmodule.h

DISTFILES += twogainblock.json
