TARGET = CostransferBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    costransferblock.cpp \
    costransferdialogbox.cpp \
    costransfermodule.cpp

HEADERS += \
    costransferblock.h \
    costransferdialogbox.h \
    costransfermodule.h

DISTFILES += costransferblock.json

FORMS += \
    costransferdialogbox.ui
