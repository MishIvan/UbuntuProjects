QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = TasksAccounting

SOURCES += \
    aboutdialog.cpp \
    autorizationform.cpp \
    main.cpp \
    mainwindow.cpp \
    projectscarddialog.cpp \
    setperioddialog.cpp \
    taskDialog.cpp \
    taskfilterdialog.cpp \
    threadexportdb.cpp \
    timerdialog.cpp \
    workrecdialog.cpp \
    worksdialog.cpp \
    worksperioddialog.cpp \
    worksreportdialog.cpp

HEADERS += \
    aboutdialog.h \
    autorizationform.h \
    mainwindow.h \
    projectscarddialog.h \
    setperioddialog.h \
    sorting.hpp \
    taskDialog.h \
    taskfilterdialog.h \
    threadexportdb.h \
    timerdialog.h \
    workrecdialog.h \
    worksdialog.h \
    worksperioddialog.h \
    worksreportdialog.h

FORMS += \
    aboutdialog.ui \
    autorizationform.ui \
    mainwindow.ui \
    projectscarddialog.ui \
    setperioddialog.ui \
    taskDialog.ui \
    taskfilterdialog.ui \
    timerdialog.ui \
    workrecdialog.ui \
    worksdialog.ui \
    worksperioddialog.ui \
    worksreportdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    MainResource.qrc

DISTFILES +=
