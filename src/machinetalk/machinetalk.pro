#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T20:31:12
#
#-------------------------------------------------

QT       += qml network
QT       -= gui
CONFIG += static
CONFIG += c++14

TARGET = machinetalk
TEMPLATE = lib

DEFINES += MACHINETALK_LIBRARY

SOURCES += $$PWD/machinetalkservice.cpp

HEADERS += $$PWD/machinetalkservice.h \
           $$PWD/machinetalk_global.h

include(../../paths.pri)
include(../common/common.pri)

win32: target.path = $$[QT_INSTALL_BINS]
!win32: target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target
