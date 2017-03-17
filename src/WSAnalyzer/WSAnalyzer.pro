#-------------------------------------------------
#
# Project created by QtCreator 2016-12-19T14:01:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WSAnalyzer
TEMPLATE = app
INCLUDEPATH += ../common ./capanalyzer ./hex ../common/mms

win32{
    DEFINES += OS_WINDOWS
    DEFINES -= UNICODE
    DEFINES += MMS_LITE
}

unix{
    DEFINES += OS_LINUX
    DEFINES -= UNICODE
    DEFINES += MMS_LITE
    LIBS=-ldl
}

include(../common/common4ws.pri)

searchlist += $$PWD/*.h
for(searchvar, searchlist) {
    hlist += $$files($$searchvar, true)
}

HEADERS += $$hlist

searchlist = $$PWD/*.cpp
for(searchvar, searchlist) {
    srclist += $$files($$searchvar, true)
}

SOURCES += $$srclist

searchlist = $$PWD/*.ui
for(searchvar, searchlist) {
    formslist += $$files($$searchvar, true)
}

FORMS += $$formslist


########################

QT += quickwidgets

INCLUDEPATH += $$[QT_INSTALL_QML]/XjUi
include($$[QT_INSTALL_QML]/XjUi/stores/stores.pri)

icons_qrc_file=$$[QT_INSTALL_QML]/icons/icons_all.qrc
exists($$icons_qrc_file){
    RESOURCES += $$icons_qrc_file
}

RESOURCES += \
    $$[QT_INSTALL_QML]/XjUi/qml/qml.qrc
