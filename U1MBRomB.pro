#-------------------------------------------------
#
# Project created by QtCreator 2012-08-02T22:32:39
#
#-------------------------------------------------
DEFINES += VERSION=\\\"1.0\\\"

TARGET = U1MBRomB
CONFIG   += qt
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    aboutdialog.cpp \
    u1mbrombsettings.cpp \
    optionsdialog.cpp \
    helpdialog.cpp \
    creditsdialog.cpp

win32:LIBS += -lwinmm
unix:LIBS += -lz

HEADERS += \
    mainwindow.h \
    aboutdialog.h \
    u1mbrombsettings.h \
    optionsdialog.h \
    helpdialog.h \
    creditsdialog.h

FORMS += \
    mainwindow.ui \
    optionsdialog.ui \
    helpdialog.ui \
    creditsdialog.ui \
    aboutdialog.ui

RESOURCES += \
    Resources.qrc \
    i18n.qrc \
    icons.qrc \
    documentation.qrc

TRANSLATIONS = i18n/U1MBRomB_tr.ts \
               i18n/U1MBRomB_de.ts \
               i18n/qt_tr.ts \
               i18n/qt_de.ts

RC_FILE = U1MBRomB.rc \

OTHER_FILES += \
    U1MBRomB.rc




































































