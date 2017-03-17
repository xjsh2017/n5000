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

SOURCES += main.cpp\
        mainwindow.cpp \
    mdichild.cpp \
    timestylesetup.cpp \
    gooseparamsetup.cpp \
    smvparamsetup.cpp \
    mmsparamsetup.cpp \
    statisticalreport.cpp \
    ../common/xjlib.cpp \
    capanalyzer/GooseAnalyzer.cpp \
    capanalyzer/IEC61850Analyzer.cpp \
    capanalyzer/Keywords61850tranformer.cpp \
    capanalyzer/Mms2AcsiWrapper.cpp \
    capanalyzer/SmvAnalyzer.cpp \
    tixml/tinystr.cpp \
    tixml/tinyxml.cpp \
    tixml/tinyxmlerror.cpp \
    tixml/tinyxmlparser.cpp \
    capanalyzer/CapPackageStatic.cpp \
    capanalyzer/CapTransformer.cpp \
    capanalyzer/LibpCapFileMnger.cpp \
    capanalyzer/PackageCovertWrapper.cpp \
    capanalyzer/Packet2Message.cpp \
    capanalyzer/ScanDissectPacketer.cpp \
    ../common/XJFile.cpp \
    ../common/LogFile.cpp \
    ../common/Lock.cpp \
    ../common/ParseASN1.cpp \
    ../common/globalfun.cpp \
    ../common/Msg2SmvPdu.cpp \
    ../common/XJString.cpp \
    capanalyzer/TsScdParserWrapper.cpp \
    detailedmessageview.cpp \
    linkbrieftableview.cpp \
    capanalyzer/anakit.cpp \
    capanalyzer/BaseDealINIFile.cpp \
    capanalyzer/cappackagesmnger.cpp \
    capanalyzer/wssysconfig.cpp \
    hex/chunks.cpp \
    hex/commands.cpp \
    hex/qhexedit.cpp \
    capanalyzer/ZipcWrapper.cpp \
    dlgpktfiltercondition.cpp \
    dlgopenmultilibcapfiles.cpp


HEADERS  += mainwindow.h \
    mdichild.h \
    timestylesetup.h \
    gooseparamsetup.h \
    smvparamsetup.h \
    mmsparamsetup.h \
    statisticalreport.h \
    capanalyzer/GooseAnalyzer.h \
    capanalyzer/IEC61850Analyzer.h \
    capanalyzer/Keywords61850tranformer.h \
    capanalyzer/Mms2AcsiWrapper.h \
    capanalyzer/SmvAnalyzer.h \
    tixml/tinystr.h \
    tixml/tinyxml.h \
    capanalyzer/CapPackageStatic.h \
    capanalyzer/CapTransformer.h \
    capanalyzer/LibpCapFileMnger.h \
    capanalyzer/PackageCovertWrapper.h \
    capanalyzer/Packet2Message.h \
    capanalyzer/ScanDissectPacketer.h \
    capanalyzer/TsScdParserWrapper.h \
    detailedmessageview.h \
    linkbrieftableview.h \
    capanalyzer/anakit.h \
    capanalyzer/BaseDealINIFile.h \
    capanalyzer/capanalyzerstruct.h \
    capanalyzer/cappackagesmnger.h \
    capanalyzer/wsconstant.h \
    capanalyzer/wssysconfig.h \
    hex/chunks.h \
    hex/commands.h \
    hex/qhexedit.h \
    capanalyzer/ZipcWrapper.h \
    dlgpktfiltercondition.h \
    dlgopenmultilibcapfiles.h

FORMS    += mainwindow.ui \
    mdichild.ui \
    timestylesetup.ui \
    gooseparamsetup.ui \
    smvparamsetup.ui \
    mmsparamsetup.ui \
    statisticalreport.ui \
    detailedmessageview.ui \
    linkbrieftableview.ui \
    dlgpktfiltercondition.ui \
    dlgopenmultilibcapfiles.ui

########################

QT += quickwidgets quick

INCLUDEPATH += $$[QT_INSTALL_QML]/XjUi
include($$[QT_INSTALL_QML]/XjUi/stores/stores.pri)

icons_qrc_file=$$[QT_INSTALL_QML]/icons/icons_all.qrc
exists($$icons_qrc_file){
    RESOURCES += $$icons_qrc_file
}

RESOURCES += \
    $$[QT_INSTALL_QML]/XjUi/qml/qml.qrc
