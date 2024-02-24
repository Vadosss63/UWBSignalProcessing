QT += core gui widgets
CONFIG += plugin c++17
DEFINES += QT_DEPRECATED_WARNINGS
TEMPLATE = lib

PATH_TO_API = $$PWD/..

NAME_FILE = $${TARGET}.json

BILD_DIR = $${files(/home/*)}/UWB/BlocksLib

IO_PATH = $${BILD_DIR}/IOBlocks
SINK_PATH = $${BILD_DIR}/SinkBlocks
SOURCE_PATH = $${BILD_DIR}/SourceBlocks

LIBS_PATH = $${files(/home/*)}/UWB/
INCLUDEPATH += $${LIBS_PATH}/include
DEPENDPATH += $${LIBS_PATH}/include
LIBS += -L$${LIBS_PATH} -lUWBframework
