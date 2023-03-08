TARGET = ChangeSizeSignalBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    changesizesignalblock.cpp \
    changesizesignaldialogbox.cpp \
    changesizesignalmodule.cpp

HEADERS += \
    changesizesignalblock.h \
    changesizesignalmodule.h \
    changesizesignaldialogbox.h

DISTFILES += changesizesignalblock.json
