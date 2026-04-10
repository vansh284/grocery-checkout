QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminpage.cpp \
    categorypage.cpp \
    helppage.cpp \
    itemdetailpage.cpp \
    main.cpp \
    mainpage.cpp \
    mainwindow.cpp \
    numpaddialog.cpp \
    page.cpp \
    paymentpage.cpp \
    receiptpage.cpp \
    welcomepage.cpp

HEADERS += \
    adminpage.h \
    cartdata.h \
    categorypage.h \
    config.h \
    helppage.h \
    itemdetailpage.h \
    mainpage.h \
    mainwindow.h \
    numpaddialog.h \
    page.h \
    paymentpage.h \
    receiptpage.h \
    style.h \
    welcomepage.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    # grocery_new_fr_FR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
