#ifdef OS_WINDOWS
#pragma execution_character_set("UTF-8")
#endif
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QString>
#include <QtWidgets>
#include "timestylesetup.h"
#include "gooseparamsetup.h"
#include "smvparamsetup.h"
#include "mmsparamsetup.h"
#include "statisticalreport.h"
#include "linkbrieftableview.h"
#include <QMessageBox>
#include "TsScdParserWrapper.h"
#include "dlgopenmultilibcapfiles.h"
#include "statisticalreport.h"
extern CapPackagesMnger ex_m_cappackagesmnger;

/**
 * @brief threadLoadScdFile 加载SCD文件多线程
 * @param param
 * @return
 */
void *threadLoadScdFile(void* param)
{
    MainWindow *mainWindowsS = (MainWindow *)param;
    mainWindowsS->loadScdFile();
    return NULL;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->OpenFile, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionTime, SIGNAL(triggered()), this, SLOT(setupTimeStyle()));    
    connect(ui->actionGOOSE, SIGNAL(triggered()), this, SLOT(setupGooseParam()));
    connect(ui->actionSV, SIGNAL(triggered()), this, SLOT(setupSmvParam()));
    connect(ui->actionMMS, SIGNAL(triggered()), this, SLOT(setupMmsParam()));
    connect(ui->actionReport, SIGNAL(triggered()), this, SLOT(statisticalReport()));
    connect(ui->actionSaveAs,SIGNAL(triggered()),this, SLOT(saveAs()));
    connect(ui->actionSCD,SIGNAL(triggered()),this, SLOT(loadScd()));
    connect(ui->actionOpenMulti,SIGNAL(triggered()),this, SLOT(openMultiFiles()));
    connect(ui->actionOpenedFiles,SIGNAL(triggered()),this, SLOT(openedFiles()));
    isScdLoading = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}
/**
 * @brief MainWindow::setupTimeStyle 时间格式设置
 */
void MainWindow::setupTimeStyle()
{
    TimeStyleSetup * timeStyle = new TimeStyleSetup(this);
    timeStyle->setWindowModality(Qt::ApplicationModal);
    timeStyle->show();

    if(timeStyle->exec() == QDialog::Rejected)
        return;

    foreach (QMdiSubWindow *window, ui->centralWidget->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        QObject::connect(this,&MainWindow::connectLinkBriefTable,mdiChild->linkView,&linkBriefTableView::FillTGirdTreForTimeStyles);
        emit connectLinkBriefTable();
    }
}
/**
 * @brief MainWindow::setupGooseParam GOOSE配置设置
 */
void MainWindow::setupGooseParam()
{
    GooseParamSetup * gooseParam = new GooseParamSetup(this);
    gooseParam->setWindowModality(Qt::ApplicationModal);
    gooseParam->show();
    if(gooseParam->exec() == QDialog::Rejected)
        return;
}
/**
 * @brief MainWindow::setupSmvParam SMV配置设置
 */
void MainWindow::setupSmvParam()
{
    SmvParamSetup * smvParam = new SmvParamSetup(this);
    smvParam->setWindowModality(Qt::ApplicationModal);
    smvParam->show();
}
/**
 * @brief MainWindow::setupMmsParam MMS配置设置
 */
void MainWindow::setupMmsParam()
{
    MmsParamSetup * mmsParam = new MmsParamSetup(this);
    mmsParam->setWindowModality(Qt::ApplicationModal);
    mmsParam->show();
}
/**
 * @brief MainWindow::statisticalReport 加载报告界面
 */
void MainWindow::statisticalReport()
{
    StatisticalReport * statisticalReport = new StatisticalReport(this);
    statisticalReport->setWindowModality(Qt::ApplicationModal);
    statisticalReport->show();
    statisticalReport->pcappackagesmnger = &ex_m_cappackagesmnger;
    statisticalReport->initReport(m_strPcapFileFullPathName);
}

/**
 * @brief MainWindow::open 打开包
 */
void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Allfiles(*.*);;pcap(*.pcap)"));
    if (!fileName.isEmpty())
        openFile(fileName);
}
/**
 * @brief MainWindow::openMultiFiles 打开合并多文件
 */
void MainWindow::openMultiFiles()
{
    MAP_TS_LIBCAP_FILE map_tslibcapfilelist;
    fillMultiList(&map_tslibcapfilelist);

}
/**
 * @brief MainWindow::openedFiles 打开合并已经打开的文件
 */
void MainWindow::openedFiles()
{
    map_filelist.clear();
    foreach (QMdiSubWindow *window, ui->centralWidget->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if(mdiChild->currentFile().indexOf("合并") != -1)
            continue;
        QFileInfo openFile = QFileInfo(mdiChild->currentFile());
        TS_LIBCAP_FILE *ptslibcapfile = new TS_LIBCAP_FILE;
        ptslibcapfile->n_filesize = openFile.size();
        ptslibcapfile->strfilename = openFile.fileName().toStdString();
        ptslibcapfile->strfilepathname = openFile.absolutePath().toStdString()+"/";
        ptslibcapfile->n_filecreatetime = openFile.lastModified().toTime_t();
        map_filelist[ptslibcapfile->n_filecreatetime] = ptslibcapfile;
    }
    fillMultiList(&map_filelist);
}
/**
 * @brief MainWindow::fillMultiList 合并多文件，并打开合并后的文件
 * @param pmap_tslibcapfilelist 信息包包头
 */
void MainWindow::fillMultiList(MAP_TS_LIBCAP_FILE* pmap_tslibcapfilelist)
{
    dlgOpenMultiLibCapfiles * multiFiles = new dlgOpenMultiLibCapfiles(this);
    multiFiles->setWindowModality(Qt::ApplicationModal);
    multiFiles->m_pmap_tslibcapfilelist = pmap_tslibcapfilelist;
    multiFiles->init();
    multiFiles->setWindowTitle("打开多文件");
    multiFiles->show();
    TS_LIBCAP_FILE * ptslibcapfile_dst = new TS_LIBCAP_FILE;
    std::map<int, TS_LIBCAP_FILE*>::iterator iter;
    QDateTime nowtime = QDateTime::currentDateTime();
    QString str = nowtime.toString("yyyy_MM_dd_hh_mm_ss"); //设置显示格式
    CLibpCapFileMnger mylibcapMngr;
    int nresult = -1;
    QString strDstFilePathName;
    if(multiFiles->exec() == QDialog::Rejected)
    {
        //清除资源
        for(iter = pmap_tslibcapfilelist->begin(); iter != pmap_tslibcapfilelist->end(); iter ++ )
        {
            delete iter->second;
        }
        pmap_tslibcapfilelist->clear();
        delete ptslibcapfile_dst;
        return;
    }
    if(pmap_tslibcapfilelist->size() == 0)//无文件选择
        return;
    iter = pmap_tslibcapfilelist->begin();
    //设置目的文件名称
    ptslibcapfile_dst->strfilepathname = iter->second->strfilepathname;//取第一个文件的路径
    ptslibcapfile_dst->strfilename = str.toStdString()+"_合并文件.pcap";
    strDstFilePathName = QString::fromStdString(ptslibcapfile_dst->strfilepathname) + QString::fromStdString(ptslibcapfile_dst->strfilename);
    ptslibcapfile_dst->strfilename = std::string( QString(ptslibcapfile_dst->strfilename.c_str()).toLocal8Bit().data());
    nresult = mylibcapMngr.Libpcap_mergerfilelist(*pmap_tslibcapfilelist,ptslibcapfile_dst);
    if(nresult == 0)//打开文件成功
    {
        loadFile(strDstFilePathName);
    }
}
/**
 * @brief MainWindow::createMdiChild 创建多文档
 * @return
 */
MdiChild *MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild;
    ui->centralWidget->addSubWindow(child);
    return child;
}
/**
 * @brief MainWindow::openFile 打开文件，判断文件是否被打开，已打开的跳转至该界面
 * @param fileName 文件绝对路径
 * @return
 */
bool MainWindow::openFile(const QString &fileName)
{
    if (QMdiSubWindow *existing = findMdiChild(fileName)) {
        ui->centralWidget->setActiveSubWindow(existing);
        return true;
    }
    const bool succeeded = loadFile(fileName);
    if (succeeded)
        statusBar()->showMessage(tr("File loaded"), 2000);
    return succeeded;
}
/**
 * @brief MainWindow::loadFile 创建多文档
 * @param fileName 文件绝对路径
 * @return
 */
bool MainWindow::loadFile(const QString &fileName)
{
    MdiChild *child = createMdiChild();
    child->loadFile(fileName);
    child->show();    
    m_strPcapFileFullPathName = fileName;
    return true;
}
/**
 * @brief MainWindow::findMdiChild 判断文件是否被打开，已打开的跳转至该界面
 * @param fileName
 * @return
 */
QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName) const
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();
    foreach (QMdiSubWindow *window, ui->centralWidget->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}
/**
 * @brief MainWindow::saveAs 另存为PCAP
 */
void MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("选择文件存放路径"),
            "",
            tr("PCAP Files (*.pcap)"));
    if (!fileName.isNull())
    {
        CLibpCapFileMnger myFileMnger;
        if(myFileMnger.SaveCapConnetion2File(&ex_m_cappackagesmnger.m_pcapconnectinfoTotal,fileName.toStdString().c_str()) == RES_FAIL)
        {
            QMessageBox::information(this,"提示","请先加载文件！");
            return;
        }
    }
    else
    {
        return;//点的是取消
    }
}
/**
 * @brief MainWindow::loadScd 加载SCD文件
 */
void MainWindow::loadScd()
{
    if(isScdLoading)
    {
        QMessageBox::information(this,"提示","SCD文件正在加载中，请稍等！");
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("选择加载的SCD文件"),
            "",
            tr("PCAP Files (*.SCD)"));
    if (!fileName.isNull())
    {
        THREAD_HANDLE m_hHande;
        THREAD_ID     m_hId = 0;
        m_strScdFileFullPathName = fileName;
        xj_thread_create(&m_hHande,&m_hId,(XJ_THREAD_ROUTINE)threadLoadScdFile,this);
    }
    else
    {
        return;//点的是取消
    }
}
/**
 * @brief MainWindow::loadScdFile 加载SCD文件
 * @return
 */
int MainWindow::loadScdFile()
{
    isScdLoading = true;
    CTsScdParserWrapper *tsScdParser = CTsScdParserWrapper::getInstance();
    if(tsScdParser->LoadscdFile_I((char *)m_strScdFileFullPathName.toStdString().c_str()))
    {
        QObject::connect(this,&MainWindow::loadScdSuccess,this,&MainWindow::loadScdInform);
        emit loadScdSuccess();
    }
    else
    {
        isScdLoading = false;
        return -1;
    }
    isScdLoading = false;
    return 0;
}
/**
 * @brief MainWindow::loadScdInform 加载完成通知主界面
 */
void MainWindow::loadScdInform()
{
    statusBar()->showMessage(tr("成功加载SCD文件!"), 3000);
}

int MainWindow::InitCfgMngr()
{/*
    try
    {
        string strDllPathName;
        strDllPathName = wsSysconfig.m_wsThirdPartSrc.cthdpartysrcrelative_path_re;
        strDllPathName.append("\\");
        strDllPathName.append(wsSysconfig.m_wsThirdPartSrc.ccfgmngrdllname);
        if(wsSysconfig.Initialize(strDllPathName.c_str()) != 0)//传入动态库，加载动态库，导出各类函数
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
    }*/
    return -1;
}

