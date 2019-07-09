#-------------------------------------------------
#
# Project created by QtCreator 2018-11-29T13:39:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ParticlesPhysicsMain
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

INCLUDEPATH += C:/Qt/gsl

CONFIG += c++17

TRANSLATIONS = lang/ParticlesPhysics_pl.ts

INCLUDEPATH += \
               extern \
               main \
               qpart

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    extern/qcgaugewidget.cpp \
    qpart/qpaintermanager.cpp \    
    qpart/qbarchart.cpp \
    qpart/qhistogram1d.cpp \
    qpart/qbardisplay.cpp \    
    qpart/qinfodisplay.cpp \
    qpart/qparticlesphysicsmanager.cpp \
    qpart/qboxpainter.cpp \
    qpart/qtextanimation.cpp \
    qpart/qtrackingplot2d.cpp \
    qpart/qinteractivelabel.cpp \
    qpart/qcirclecontrol.cpp \
    main/particlesphysicsmanager.cpp \
    main/particle.cpp \
    main/cluster.cpp \
    main/histogram1d.cpp \
    main/barchart.cpp \    
    main/planedivider.cpp \
    main/bardisplay.cpp \     
    main/planearea.cpp \
    main/langmanager.cpp \
    dialogabout.cpp

   
HEADERS += \
    mainwindow.h \
    extern/random.hpp \
    extern/qcgaugewidget.h \
    qpart/qpaintermanager.h \
    qpart/qbardisplay.h \    
    qpart/qinfodisplay.h \
    qpart/qbarchart.h \
    qpart/qhistogram1d.h \
    qpart/qparticlesphysicsmanager.h \
    qpart/qboxpainter.h \
    qpart/qtextanimation.h \
    qpart/qtrackingplot2d.h \
    qpart/qinteractivelabel.h \
    qpart/qcirclecontrol.h \
    main/definitions.h \
    main/particlesphysicsmanager.h \    
    main/vector2d.h \
    main/particle.h \
    main/cluster.h \
    main/sline2d.h \
    main/particlesmath.h \
    main/histogram1d.h \
    main/barchart.h \
    main/planedivider.h \
    main/bardisplay.h \  
    main/planearea.h \
    main/langmanager.h \
    dialogabout.h
    
    
FORMS += \
        mainwindow.ui \
    dialogabout.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    graphics.qrc
