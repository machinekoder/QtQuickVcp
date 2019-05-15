TEMPLATE = lib
QT += qml quick network

uri = Machinekit.VideoView
include(../plugin.pri)

# Input
SOURCES += \
    plugin/plugin.cpp \
    mjpegstreamerclient.cpp

HEADERS += \
    plugin/plugin.h \
    mjpegstreamerclient.h

QML_INFRA_FILES = \
    plugin/qmldir

QML_DESIGNER_FILES = \
    designer

include(../deployment.pri)
