QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adduserdialog.cpp \
    autorizationform.cpp \
    changepassworddialog.cpp \
    changestatusdialog.cpp \
    closeuserdialog.cpp \
    flightdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    ticketcarddialog.cpp \
    ticketdialog.cpp \
    towndialog.cpp

HEADERS += \
    adduserdialog.h \
    autorizationform.h \
    changepassworddialog.h \
    changestatusdialog.h \
    closeuserdialog.h \
    flightdialog.h \
    mainwindow.h \
    ticketcarddialog.h \
    ticketdialog.h \
    towndialog.h

FORMS += \
    adduserdialog.ui \
    autorizationform.ui \
    changepassworddialog.ui \
    changestatusdialog.ui \
    closeuserdialog.ui \
    flightdialog.ui \
    mainwindow.ui \
    ticketcarddialog.ui \
    ticketdialog.ui \
    towndialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    MainResource.qrc

DISTFILES +=
