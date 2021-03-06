#-------------------------------------------------
#
# Project created by QtCreator 2018-05-22T09:59:15
#
#-------------------------------------------------

QT       += core gui \
                    multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GRAVITY
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
    login.cpp \
    user.cpp \
    scene.cpp \
    taco.cpp \
    element.cpp \
    planet.cpp \
    level.cpp \
    velocity.cpp \
    physics.cpp \
    targetbarrier.cpp \
    detail.cpp \
    extra.cpp

HEADERS += \
        mainwindow.h \
    login.h \
    user.h \
    scene.h \
    taco.h \
    element.h \
    planet.h \
    level.h \
    velocity.h \
    physics.h \
    targetbarrier.h \
    detail.h \
    extra.h

FORMS += \
        mainwindow.ui \
    login.ui \
    scene.ui \
    velocity.ui \
    detail.ui

RC_ICONS = ICON.ico

RESOURCES += \
    resources.qrc

DISTFILES += \
    Users_doc.txt \
    Level_doc.txt \
    Game_doc.txt
