#-------------------------------------------------
#
# Project created by QtCreator 2020-05-07T13:52:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kurs_orel2
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
    adding.cpp \
    compare.cpp \
    del_free.cpp \
    dialog_add.cpp \
    finding.cpp \
    dialog_addctr.cpp \
    dialog_addeps.cpp \
    dialog_delzamfind.cpp \
    funcs_minmax.cpp \
    func_files.cpp

HEADERS += \
        mainwindow.h \
    types_kurs.h \
    functions_kurs.h \
    dialog_add.h \
    dialog_addctr.h \
    dialog_addeps.h \
    dialog_delzamfind.h \
    types_minmax.h

FORMS += \
        mainwindow.ui \
    dialog_add.ui \
    dialog_addctr.ui \
    dialog_addeps.ui \
    dialog_delzamfind.ui

RESOURCES += \
    res.qrc
