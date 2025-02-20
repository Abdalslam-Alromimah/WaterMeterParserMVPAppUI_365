# Existing part of the .pro file
QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WaterMeterFileParser
TEMPLATE = app
CONFIG += c++20

SOURCES += \
    main.cpp \
    model/databasemodel.cpp \
    model/dbmanager.cpp \
    model/filedata.cpp \
    model/fileparsermodel.cpp \
    model/helpbrowser.cpp \
    presenter/watermeterparser_presenter.cpp \
    view/aboutform.cpp \
    view/loginform.cpp \
    view/watermeterparser_view.cpp

HEADERS += \
    config.h \
    interfaces/basemodel.h \
    interfaces/basepresenter.h \
    interfaces/baseview.h \
    model/WaterMeterParser_Model.h \
    model/basemodel.h \
    model/databasemodel.h \
    model/dbmanager.h \
    model/filedata.h \
    model/helpbrowser.h \
    presenter/basepresenter.h \
    presenter/watermeterparser_presenter.h \
    view/aboutform.h \
    view/baseview.h \
    view/loginform.h \
    view/watermeterparser_view.h \
    view/watermeterparser_view_signals.h

FORMS += \
    view/aboutform.ui \
    view/loginform.ui \
    view/watermeterparser_view.ui

RESOURCES += \
    resources.qrc

CONFIG += file_copies
COPIES += resourcefiles

resourcefiles.files = $$PWD/resources/*
resourcefiles.path = $$OUT_PWD/debug/resources

# Deployment rules
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# The script (PowerShell)
win32 {
    INSTALLS += sqlite_installation_step
    sqlite_installation_step.path = $$OUT_PWD/debug/resources
    sqlite_installation_step.files = sqlite_check_install.ps1
}

