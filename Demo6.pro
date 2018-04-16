#-------------------------------------------------
#
# Project created by QtCreator 2017-11-16T10:43:13
#
#-------------------------------------------------

QT       += core gui
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo6
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    EGM/egm.pb.cc \
    EGM/robot_egm.cpp \
    Kinematics/frame.cpp \
    Kinematics/link.cpp \
    Kinematics/robot.cpp \
    Threads/vive_thread.cpp \
    Vive/shared/Matrices.cpp \
    Vive/LighthouseTracking.cpp \
    main.cpp \
    mainwindow.cpp \
    Controller/controller.cpp \
    Threads/egm_thread.cpp

HEADERS += \
        mainwindow.h \
    EGM/egm.pb.h \
    EGM/robot_egm.h \
    Kinematics/frame.h \
    Kinematics/link.h \
    Kinematics/robot.h \
    Threads/vive_thread.h \
    util/disp.h \
    util/main_def.h \
    Vive/shared/compat.h \
    Vive/shared/Matrices.h \
    Vive/shared/strtools.h \
    Vive/shared/Vectors.h \
    Vive/LightHouseTracking.h \
    mainwindow.h \
    Controller/controller.h \
    Threads/egm_thread.h

FORMS += \
        mainwindow.ui


INCLUDEPATH += "C:\Eigen"

LIBS += -LC:/openvr/bin/win64/RelWithDebInfo/ -lopenvr_api64

INCLUDEPATH += C:/openvr/bin/win64/RelWithDebInfo
DEPENDPATH += C:/openvr/bin/win64/RelWithDebInfo


LIBS += -LC://protobuf-3.4.1//cmake//build//Release// -llibprotobuf

INCLUDEPATH += C://protobuf-3.4.1//cmake//build//include
DEPENDPATH += C://protobuf-3.4.1//cmake//build//include

