TARGET = StatisticaleStimateBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

SOURCES += \
    statisticalestimateblock.cpp \
    statisticalestimatedialogbox.cpp \
    statisticalestimatemodule.cpp

HEADERS += \
    statisticalestimateblock.h \
    statisticalestimatedialogbox.h \
    statisticalestimatemodule.h \
    showstatisticalestimate.h

DISTFILES += statisticalestimateblock.json
