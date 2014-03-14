#Directories
TOP = /home/mah
LINUXCNCDIR = $$TOP/linuxcnc
PROTOGEN = $$LINUXCNCDIR/src/middleware/generated
NZMQTDIR = $$TOP/nzmqt
NZMQT_ANDROID_DIR = $$NZMQTDIR/bin/android/
ZEROMQ_ANDROID_DIR = /opt/zeromq-android
PROTOBUF_ANDROID_DIR = /home/alexander/projects/tmp/protobuf-2.5.0
NZMQT_X86_DIR = $$NZMQTDIR/bin/

# Add more folders to ship with the application, here
folder_01.source = qml/QtQuickVcp
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    hal/qpin.cpp \
    hal/qcomponent.cpp \
    $$PROTOGEN/canon.pb.cc \
    $$PROTOGEN/emcclass.pb.cc \
    $$PROTOGEN/log.pb.cc \
    $$PROTOGEN/message.pb.cc \
    $$PROTOGEN/motcmds.pb.cc \
    $$PROTOGEN/nanopb.pb.cc \
    $$PROTOGEN/object.pb.cc \
    $$PROTOGEN/rtapicommand.pb.cc \
    $$PROTOGEN/rtapi_message.pb.cc \
    $$PROTOGEN/rtmessage.pb.cc \
    $$PROTOGEN/task.pb.cc \
    $$PROTOGEN/test.pb.cc \
    $$PROTOGEN/types.pb.cc \
    $$PROTOGEN/value.pb.cc


INCLUDEPATH += \
    $$NZMQTDIR/nzmqt/include \
    $$NZMQTDIR/externals/include

android: {
LIBS += -L$$ZEROMQ_ANDROID_DIR/lib/ \
        -L$$NZMQT_ANDROID_DIR \
        -L$$PROTOBUF_ANDROID_DIR/src/.libs/
INCLUDEPATH += $$ZEROMQ_ANDROID_DIR/include/ \
            $$PROTOBUF_ANDROID_DIR/src/
}
else: {
LIBS += -L$$NZMQT_X86_DIR
}

LIBS += -lzmq -lnzmqtd -lprotobuf

# Installation path
# target.path =


# Please do not modify the following two lines. Required for deployment.
include(qtquick2controlsapplicationviewer/qtquick2controlsapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    hal/qpin.h \
    hal/qcomponent.h \
    $$PROTOGEN/canon.pb.h \
    $$PROTOGEN/emcclass.pb.h \
    $$PROTOGEN/log.pb.h \
    $$PROTOGEN/message.pb.h \
    $$PROTOGEN/motcmds.pb.h \
    $$PROTOGEN/nanopb.pb.h \
    $$PROTOGEN/object.pb.h \
    $$PROTOGEN/rtapicommand.pb.h \
    $$PROTOGEN/rtapi_message.pb.h \
    $$PROTOGEN/rtmessage.pb.h \
    $$PROTOGEN/task.pb.h \
    $$PROTOGEN/test.pb.h \
    $$PROTOGEN/types.pb.h \
    $$PROTOGEN/value.pb.h
