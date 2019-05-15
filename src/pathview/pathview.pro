TEMPLATE = lib
QT += qml quick network

uri = Machinekit.PathView
include(../plugin.pri)

include(../common/common.pri)
include(../machinetalk/machinetalk.pri)

# Input
SOURCES += \
    plugin/plugin.cpp \
    qglcamera.cpp \
    gcodeprogramitem.cpp \
    gcodeprogramloader.cpp \
    gcodeprogrammodel.cpp \
    glcanvas.cpp \
    glcubeitem.cpp \
    glcylinderitem.cpp \
    glitem.cpp \
    glpathitem.cpp \
    gllight.cpp \
    glsphereitem.cpp \
    glview.cpp \
    previewclient.cpp \
    gllathetoolitem.cpp

HEADERS += \
    plugin/plugin.h \
    gcodeprogramitem.h \
    gcodeprogramloader.h \
    gcodeprogrammodel.h \
    glcanvas.h \
    glcubeitem.h \
    glcylinderitem.h \
    qglcamera.h \
    glitem.h \
    gllight.h \
    glpathitem.h \
    glsphereitem.h \
    glview.h \
    previewclient.h \
    gllathetoolitem.h \
    pi_constants.h

RESOURCES += \
    shaders.qrc \
    pathview.qrc

QML_FILES = \
    $$PWD/BoundingBox3D.qml \
    $$PWD/ClearBackplotAction.qml \
    $$PWD/Coordinate3D.qml \
    $$PWD/GCodeSync.qml \
    $$PWD/Grid3D.qml \
    $$PWD/PathView3D.qml \
    $$PWD/PathViewAction.qml \
    $$PWD/PathViewCore.qml \
    $$PWD/PathViewObject.qml \
    $$PWD/ProgramExtents3D.qml \
    $$PWD/ProgramOffsets3D.qml \
    $$PWD/SourceView.qml \
    $$PWD/ViewModeAction.qml \
    $$PWD/ZoomInAction.qml \
    $$PWD/ZoomOutAction.qml \
    $$PWD/ZoomOriginalAction.qml

QML_INFRA_FILES = \
    $$QML_FILES \
    plugin/qmldir

OTHER_FILES += \
    SimpleVertex.glsl \
    SimpleFragment.glsl \
    LineVertexShader.glsl \
    LineFragmentShader.glsl \
    TextFragmentShader.glsl \
    TextVertexShader.glsl

include(../deployment.pri)

TRANSLATIONS_PATH = $$PWD/translations
include(../translation.pri)
