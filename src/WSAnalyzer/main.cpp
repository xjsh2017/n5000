#include "mainwindow.h"
#include <QApplication>
#include "wssysconfig.h"
#include "IEC61850Analyzer.h"
#include "TsScdParserWrapper.h"
#include "Keywords61850tranformer.h"
#include "ZipcWrapper.h"
#include <QDebug>

/**
 * @brief InitCfgMngr 初始化配置文件管理mms2acsi.dll
 * @return
 */
int InitCfgMngr()
{
    try
    {
        CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
        string strDllPathName;
        strDllPathName = wsSysconfig->m_wsThirdPartSrc.cthdpartysrcrelative_path_re;
        strDllPathName.append("/");
        strDllPathName.append(wsSysconfig->m_wsThirdPartSrc.ccfgmngrdllname);
        if(wsSysconfig->Initialize(strDllPathName.c_str()) != 0)//传入动态库，加载动态库，导出各类函数
        {
            //CString strTemp;
            //strTemp.Format("加载动态库:%s失败",strDllPathName);
            //AfxMessageBox(strTemp);
            return -1;
        }
        else
        {
            ;
        }
        return 0;
    }
    catch (...)
    {
        ;
    }
    return -1;

}

/**
 * @brief IniIec61850Analyzer 初始化mms解析到acis静态态库mms2acsi.dll
 * @return
 */
int IniIec61850Analyzer()
{
    string strDllPathName;
    try
    {
        CIEC61850Analyzer * iecAnalyzer = CIEC61850Analyzer::getInstance();
        CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
        //加载采样分析动态库
        strDllPathName = wsSysconfig->m_wsThirdPartSrc.cthdpartysrcrelative_path_re;
        strDllPathName.append("/");
        strDllPathName.append(wsSysconfig->m_wsThirdPartSrc.cmms2acsistaticdllname);
        if(iecAnalyzer->InitializeMms2AcsiAnalyzer(strDllPathName.c_str()) != 0)//传入动态库，加载动态库，导出各类函数
        {
            ;
        }
        else
        {
            ;
        }
        strDllPathName.clear();
        //加载goose分析动态库
        strDllPathName = wsSysconfig->m_wsThirdPartSrc.cthdpartysrcrelative_path_re;
        strDllPathName.append("/");
        strDllPathName.append(wsSysconfig->m_wsThirdPartSrc.gooseanalyzedllname);
        if(iecAnalyzer->InitializeGooseAnalyzer(strDllPathName.c_str()) != 0)//传入动态库，加载动态库，导出各类函数
        {
            ;
        }
        else
        {
            ;
        }
        strDllPathName.clear();
        //加载采样分析动态库
        strDllPathName = wsSysconfig->m_wsThirdPartSrc.cthdpartysrcrelative_path_re;
        strDllPathName.append("/");
        strDllPathName.append(wsSysconfig->m_wsThirdPartSrc.smvanalyzedllname);
        if(iecAnalyzer->InitializeSmvAnalyzer(strDllPathName.c_str()) != 0)//传入动态库，加载动态库，导出各类函数
        {
            ;
        }
        else
        {
            ;
        }
        strDllPathName.clear();
        return 0;
    }
    catch (...)
    {
       ;
    }
    return -1;
}
/**
 * @brief InitScdParser 初始化scd中文库scdparser.dll
 * @return
 */
int InitScdParser()
{
    string strDllPathName;
    try
    {
        //加载网络包解析器动态库
        CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
        CTsScdParserWrapper *tsScdParser = CTsScdParserWrapper::getInstance();
        strDllPathName = wsSysconfig->m_wsThirdPartSrc.cthdpartysrcrelative_path_re;
        strDllPathName.append("/");
        strDllPathName.append(wsSysconfig->m_wsThirdPartSrc.stsscdparserdllname);
        if(tsScdParser->Initialize(strDllPathName.c_str()) != 0)//传入动态库，加载动态库，导出各类函数
        {
            return -1;
        }
        else
        {
            ;
        }
        return 0;
    }
    catch (...)
    {
        ;
    }
    return -1;
}
/**
 * @brief IniKeyWords61850tranformer 初始化数据字典wsdictionary.xml
 * @return
 */
int IniKeyWords61850tranformer()
{
    try
    {
        CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
        CKeywords61850tranformer *keywords61850Ts = CKeywords61850tranformer::getInstance();
        string strDllPathName;
        strDllPathName = wsSysconfig->m_wsSysCommcfg.c61850keywordtransformerpath;
        strDllPathName.append("/");
        strDllPathName.append(wsSysconfig->m_wsSysCommcfg.c61850keywordtransformerfilename);
        if(keywords61850Ts->Initialize(strDllPathName) != 0)//传入动态库，加载动态库，导出各类函数
        {
            return -1;
        }
        else
        {
            ;
        }
        return 0;
    }
    catch (...)
    {
        ;
    }
    return -1;
}
/**
 * @brief InitZipcWrapper 初始化压缩解压缩动态库名zipc.dll
 * @return
 */
int InitZipcWrapper()
{
    string strDllPathName;
    try
    {
        CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
        CZipcWrapper * unzipC = CZipcWrapper::getInstance();
        //加载采样分析动态库
        strDllPathName = wsSysconfig->m_wsThirdPartSrc.cthdpartysrcrelative_path_re;
        strDllPathName.append("/");
        strDllPathName.append(wsSysconfig->m_wsThirdPartSrc.czipcdllname);
        if(unzipC->Initialize(strDllPathName.c_str()) != 0)//传入动态库，加载动态库，导出各类函数
        {
            return -1;
        }
        else
        {
            ;
        }
        return 0;
    }
    catch (...)
    {
        ;
    }
    return -1;
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));
    w.show();
    InitCfgMngr();
    IniIec61850Analyzer();
    InitScdParser();
    IniKeyWords61850tranformer();
    InitZipcWrapper();
    if(argv[1]!=NULL)
        w.openFile(QString(argv[1]));
    return a.exec();
}

