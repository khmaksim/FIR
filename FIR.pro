#-------------------------------------------------
#
# Project created by QtCreator 2019-04-29T11:00:15
#
#-------------------------------------------------

QT       += core gui sql quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FIR
TEMPLATE = app
SUBDIRS = delegate \
            model

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# QXlsx
#QXLSX_PARENTPATH = ./
#QXLSX_HEADERPATH = ./header/
#QXLSX_SOURCEPATH = ./source/
#include(./QXlsx.pri)


INCLUDEPATH += delegate \
                model

SOURCES += \
#    importdialog.cpp \
        main.cpp \
    obstraclesform.cpp \
    qgroupheaderview.cpp \
#    filterpanel.cpp \
#    sidebar.cpp \
#    flowlayout.cpp \
    databaseaccess.cpp \
    mapview.cpp \
    mainwindow.cpp \
    delegate\listitemdelegate.cpp \
    delegate\checkboxitemdelegate.cpp \
    model\searchmodel.cpp \
    model\tablemodel.cpp \
    model\sortsearchfilterobstraclemodel.cpp \
    settingsdialog.cpp

HEADERS += \
#    importdialog.h \
    obstraclesform.h \
    qgroupheaderview.h \
#    filterpanel.h \
#    sidebar.h \
#    flowlayout.h \
    databaseaccess.h \
    mapview.h \
    mainwindow.h \
    delegate\listitemdelegate.h \
    delegate\checkboxitemdelegate.h \
    model\searchmodel.h \
    model\tablemodel.h \
    model\sortsearchfilterobstraclemodel.h \
    helper.h \
    settingsdialog.h

FORMS += \
    importdialog.ui \
    obstraclesform.ui \
    settingsdialog.ui \
    sidebar.ui \
    mainwindow.ui

RESOURCES += fir.qrc

TRANSLATIONS = fir_ru.ts

RC_ICONS = ./res/img/appico.ico
RC_FILE = app.rc

DISTFILES += app.rc \
    app.rc
