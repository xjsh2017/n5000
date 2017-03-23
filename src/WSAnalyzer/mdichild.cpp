#ifdef OS_WINDOWS
#pragma execution_character_set("UTF-8")
#endif
#include "mdichild.h"
#include <QFileDialog>
#include "wsconstant.h"
#include <ctime>
#include "wssysconfig.h"
#include "acsistruct.h"
#include "AnalyzeOnline_STRUCT.h"
#include "ZipcWrapper.h"
#include <QDebug>

extern CCapTransformer * g_capTransformer;  //cap文件生成的txt文件解析器
//CZipcWrapper              m_zicpWrapper;              //解压缩动态库封装
extern CScanDissectPacketer      *g_ScanDissectPacketer;      //用于数据包的解析

CKeywords61850tranformer *keywords61850Ts = CKeywords61850tranformer::getInstance();
void MdiChild::test()
{/*
    m_cappackagesmnger.m_pcapconnectinfoTotal.nusetype = 1;
    for(int i = 0 ; i < 7 ; i++)
    {
        CAPCONNECTINFO * pconnection = new CAPCONNECTINFO;
        CAPMSGGININFO * pmsgginifo = new CAPMSGGININFO;
        if(i==0){
            pconnection->nconnectapptype = PROTOAPPTYPE_GOOSE;
            pconnection->ncapp_id = 56506;
            strcpy(pconnection->csrc2_mac,"AB-CD-EF-GH-IJ-KL");
            pconnection->neventpackages = 2;
            m_cappackagesmnger.m_pcapconnetionfoTotal_GOOSE = *pconnection;
            m_cappackagesmnger.m_pcapconnetionfoTotal_GOOSE.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(0, pmsgginifo));
            m_cappackagesmnger.m_pcapconnetionfoTotal_GOOSE.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(1, pmsgginifo));
            m_cappackagesmnger.m_pcapconnetionfoTotal_GOOSE.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(2, pmsgginifo));
            m_cappackagesmnger.m_pcapconnetionfoTotal_GOOSE.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(3, pmsgginifo));
            m_cappackagesmnger.m_pcapconnetionfoTotal_GOOSE.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(4, pmsgginifo));
        }
        else if(i==1){
            pconnection->nconnectapptype = PROTOAPPTYPE_SMV92;
            pconnection->ncapp_id = 43981;
            strcpy(pconnection->csrc2_mac,"2B-2D-2F-2H-2J-2L");
            strcpy(pconnection->csrc1_mac,"FB-2D-2F-2H-2J-2L");
            pconnection->nerrpackages = 2;
            m_cappackagesmnger.m_pcapconnetionfoTotal_SV = *pconnection;
            m_cappackagesmnger.m_pcapconnetionfoTotal_SV.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(0, pmsgginifo));
            m_cappackagesmnger.m_pcapconnetionfoTotal_SV.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(1, pmsgginifo));

        }
        else if(i==2){
            pconnection->nconnectapptype = PROTOAPPTYPE_MMS;
            strcpy(pconnection->csrc1_ip,"10.123.16.50");
            strcpy(pconnection->csrc2_ip,"10.123.16.250");
            pconnection->nsrc1packages = 22;
            pconnection->nsrc2packages = 33;
            m_cappackagesmnger.m_pcapconnetionfoTotal_MMS = *pconnection;
            m_cappackagesmnger.m_pcapconnetionfoTotal_MMS.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(0, pmsgginifo));
        }
        else if(i==3){
            pconnection->nconnectapptype = PROTOAPPTYPE_UDP;
            strcpy(pconnection->csrc1_mac,"ZZ-2D-2F-5H-2J-3L");
            strcpy(pconnection->csrc2_mac,"2B-2D-2F-2H-2J-2L");
            pconnection->nsrc1packages = 34;
            pconnection->nsrc2packages = 45;
            m_cappackagesmnger.m_pcapconnetionfoTotal_UDP = *pconnection;
            m_cappackagesmnger.m_pcapconnetionfoTotal_UDP.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(0, pmsgginifo));
        }
        else if(i==4){
            pconnection->nconnectapptype = PROTOAPPTYPE_TCP;
            strcpy(pconnection->csrc1_mac,"SS-2D-2F-4H-2J-4L");
            strcpy(pconnection->csrc2_mac,"4B-2D-2F-2H-2J-2L");
            pconnection->nsrc1packages = 87;
            pconnection->nsrc2packages = 23;
            m_cappackagesmnger.m_pcapconnetionfoTotal_TCP = *pconnection;
            m_cappackagesmnger.m_pcapconnetionfoTotal_TCP.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(0, pmsgginifo));
        }
        else if(i==5){
            pconnection->nconnectapptype = PROTOAPPTYPE_ARP;
            strcpy(pconnection->csrc1_mac,"QQ-2D-2F-6H-2J-5L");
            strcpy(pconnection->csrc2_mac,"5B-2D-2F-2H-2J-2L");
            pconnection->nsrc1packages = 64;
            pconnection->nsrc2packages = 24;
            m_cappackagesmnger.m_pcapconnetionfoTotal_ARP = *pconnection;
            m_cappackagesmnger.m_pcapconnetionfoTotal_ARP.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(0, pmsgginifo));
        }
        else if(i==6){
            pconnection->nconnectapptype = PROTOAPPTYPE_OTHER;
            strcpy(pconnection->csrc1_mac,"PP-2D-2F-9H-2J-6L");
            strcpy(pconnection->csrc2_mac,"7B-2D-2F-2H-2J-2L");
            pconnection->nsrc1packages = 86;
            pconnection->nsrc2packages = 12;
            m_cappackagesmnger.m_pcapconnetionfoTotal_OTHER = *pconnection;
            m_cappackagesmnger.m_pcapconnetionfoTotal_OTHER.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(0, pmsgginifo));
            m_cappackagesmnger.m_pcapconnetionfoTotal_OTHER.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(1, pmsgginifo));
            m_cappackagesmnger.m_pcapconnetionfoTotal_OTHER.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(2, pmsgginifo));
        }
        pmsgginifo->nseq = i;
        pmsgginifo->nseconds_utc_tmstamp = 1482741930;
        pmsgginifo->nus_tmstamp = 123456;
        pmsgginifo->ftime_relative = 0.00025;
        pmsgginifo->ftime_delta = 0.00025;
        string ss;
        char bb[2];
        sprintf(bb,"%d",i);
        ss = "序号:";
        ss.append(bb);
        strcpy(pmsgginifo->ccontent,ss.c_str());
        strcpy(pmsgginifo->csrc_mac,"2B-2D-2F-2H-2J-2L");
        strcpy(pmsgginifo->cdst_mac,"FB-2D-2F-2H-2J-2L");
        pconnection->nseq = i;
        pconnection->map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(0, pmsgginifo));
        pconnection->map_capmsginfo_error.insert(std::map <int, CAPMSGGININFO*> :: value_type(0, pmsgginifo));
        pconnection->map_capmsginfo_error.insert(std::map <int, CAPMSGGININFO*> :: value_type(1, pmsgginifo));
        pconnection->map_capmsginfo_event.insert(std::map <int, CAPMSGGININFO*> :: value_type(0, pmsgginifo));
        pconnection->map_capmsginfo_event.insert(std::map <int, CAPMSGGININFO*> :: value_type(1, pmsgginifo));
        m_cappackagesmnger.m_mapcapconnectionfo.insert(std::map <int, CAPCONNECTINFO*> :: value_type(i, pconnection));
        m_cappackagesmnger.m_pcapconnectinfoTotal.map_capmsginfo_filter.insert(std::map <int, CAPMSGGININFO*> :: value_type(i, pmsgginifo));
        m_cappackagesmnger.m_pcapconnectinfoTotal.map_capmsginfo.insert(std::map <int, CAPMSGGININFO*> :: value_type(i, pmsgginifo));
    }
*/
}

MdiChild::MdiChild(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MdiChild)
{
    ui->setupUi(this);
    itemRoot = NULL;
    itemLeafMms = NULL;
    itemLeafGs = NULL;
    itemLeafSmv = NULL;
    itemLeaf1588 = NULL;
    itemLeafUdp = NULL;
    itemLeafTcp = NULL;
    itemLeafArp = NULL;
    itemLeafOther = NULL;
    linkView = NULL;
    connect(ui->linkTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(OnMySelectTreeItem(QTreeWidgetItem*,int)));
    //ui->linkBriefTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置表格表头自适应
    test();
}

MdiChild::~MdiChild()
{
    delete ui;
}

bool MdiChild::loadFile(const QString &fileName)
{
    QString getFileName = fileName;
    if (getFileName.endsWith(QString(".zip"))||getFileName.endsWith(QString(".tzip"))||getFileName.endsWith(QString(".zipx")))
    {
        CZipcWrapper * unzipC = CZipcWrapper::getInstance();
        unzipC->UnZip_MINILZO_I(fileName.toStdString().c_str(),NULL);
        int n = getFileName.indexOf(".");
        getFileName = getFileName.mid(0,n);
        getFileName.append(".pcap");
    }
    if (getFileName.endsWith(QString(".pcap")))
    {
        m_anakit = new AnaKit();
        m_anakit->Initialize();
        //获取装置名称与采集端口
        QFileInfo openFile = QFileInfo(fileName);
        QString strPort;
        strPort.sprintf("%s",openFile.fileName().toStdString().c_str());
        int nPort = atoi(strPort.toStdString().c_str());
        CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
        SNIFFER* pSniffer = wsSysconfig->I_GET_SNIFFER(nPort);
        if(pSniffer != NULL)
            strcpy(m_anakit->m_cappackagesmnger.m_capparserninfo.crecoriedname,pSniffer->csniffername);
        SNIFFER_PORT* pPort = wsSysconfig->I_GET_SNIFFERPORT(nPort);
        if(pPort != NULL)
        {
            strcpy(m_anakit->m_cappackagesmnger.m_capparserninfo.crecorportname,pPort->cportname);
        }
        strcpy(m_anakit->m_cappackagesmnger.m_capparserninfo.cparserfilename,openFile.fileName().toStdString().c_str());

        bool ret = m_anakit->OpenCapFileAndParse(std::string( getFileName.toLocal8Bit().data()));
        if (ret)
        {
            fillLinkTree();
            //RefreshConnectDetail(NULL,CAPSHOW_DATATYE_ALL,PROTOAPPTYPE_TOTAL);
            if(linkView == NULL)
            {
                linkView = new linkBriefTableView();
                ui->splitter->addWidget(linkView);
                ui->splitter->setStretchFactor(0,2);
                ui->splitter->setStretchFactor(1,8);
            }
            QObject::connect(this,&MdiChild::wsUpdateWSAnalData,linkView,&linkBriefTableView::UpdateWaveAnalDataModel);
            QObject::connect(this,&MdiChild::wsWriteLinkMsgDetail,linkView,&linkBriefTableView::receiveMsgDetail);

            emit wsWriteLinkMsgDetail(&m_anakit->m_cappackagesmnger,NULL,CAPSHOW_DATATYE_ALL,PROTOAPPTYPE_TOTAL);
        }
        else
        {
            ;
        }
    }
    setCurrentFile(getFileName);
    return true;
}

void MdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowModified(false);
    setWindowTitle(QFileInfo(fileName).fileName() + "[*]");
    //RefreshConnectDetail(NULL,CAPSHOW_DATATYE_ALL,IEC61850_ETHER_TYPE_SMV);
    //RefreshConnectDetail(NULL,CAPSHOW_DATATYE_ALL,IEC61850_ETHER_TYPE_MMS);
}

void MdiChild::fillLinkTree()
{
    QString m_napppackages = QString::number(m_anakit->m_cappackagesmnger.m_capparserninfo.napppackages, 10);
    QStringList rootTree;
        rootTree << "全部报文[总帧数："+m_napppackages+"]";//树形统计头根节点
    itemRoot = new QTreeWidgetItem(ui->linkTreeWidget,rootTree);
    itemRoot->setData(0,Qt::UserRole,QVariant::fromValue(PROTOAPPTYPE_TOTAL));
    //写入链路信息
     std::map <int, CAPCONNECTINFO*>::iterator iter;
     CAPCONNECTINFO * pCapconnectinfo = NULL;
     for(iter = m_anakit->m_cappackagesmnger.m_mapcapconnectionfo.begin(); iter != m_anakit->m_cappackagesmnger.m_mapcapconnectionfo.end(); iter ++ )
     {
         pCapconnectinfo = iter->second;
         if(pCapconnectinfo == NULL)
             continue;
         if(pCapconnectinfo->map_capmsginfo.size() == 0)
             continue;
         //增加链路
         fillNetConnectInfo(pCapconnectinfo);
     }
     ui->linkTreeWidget->expandItem(itemRoot);
     //ui->linkTreeWidget->expandAll();
     ui->linkTreeWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
     ui->linkTreeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
     QHeaderView *pHeader=ui->linkTreeWidget->header();
     pHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
     pHeader->setStretchLastSection(false);

}

void MdiChild::fillNetConnectInfo(CAPCONNECTINFO* pcapconnectinfo)
{
    QTreeWidgetItem * itemLeaf = GetParentByEtherType(pcapconnectinfo->nconnectapptype);
    QStringList rootTree = GetConnectDesc(pcapconnectinfo);
    QTreeWidgetItem * itemLeafChild = new QTreeWidgetItem(itemLeaf,rootTree);
    itemLeafChild->setData(0,Qt::UserRole,QVariant::fromValue(-7));
    itemLeafChild->setData(1,Qt::UserRole,QVariant::fromValue(pcapconnectinfo->nseq));
    itemLeafChild->setData(2,Qt::UserRole,QVariant::fromValue(PROTOAPPTYPE_TOTAL));
    //int ss = itemLeafChild->data(0,Qt::UserRole).value< int >();
    //qDebug() << QString::number(itemLeafChild->data(1,Qt::UserRole).value< int >(),10);
    if(pcapconnectinfo->neventpackages > 0)
    {
        QStringList leafTree;
        leafTree<< "事件："+QString::number(pcapconnectinfo->neventpackages,10);
        QTreeWidgetItem * itemLeafChildEvent = new QTreeWidgetItem(itemLeafChild,leafTree);
        itemLeafChildEvent->setData(0,Qt::UserRole,QVariant::fromValue(-7));
        itemLeafChildEvent->setData(1,Qt::UserRole,QVariant::fromValue(pcapconnectinfo->nseq));
        itemLeafChildEvent->setData(2,Qt::UserRole,QVariant::fromValue(PROTOAPPTYPE_EVENT));
        ui->linkTreeWidget->expandItem(itemLeafChild);
    }
    if(pcapconnectinfo->nerrpackages > 0)
    {
        QStringList leafTree;
        leafTree<< "异常："+QString::number(pcapconnectinfo->nerrpackages,10);
        QTreeWidgetItem * itemLeafChildError = new QTreeWidgetItem(itemLeafChild,leafTree);
        itemLeafChildError->setData(0,Qt::UserRole,QVariant::fromValue(-7));
        itemLeafChildError->setData(1,Qt::UserRole,QVariant::fromValue(pcapconnectinfo->nseq));
        itemLeafChildError->setData(2,Qt::UserRole,QVariant::fromValue(PROTOAPPTYPE_ERROR));
        ui->linkTreeWidget->expandItem(itemLeafChild);
    }
    if(pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_SMV92 || pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_GOOSE ||
        pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_MMS)
    {
        ui->linkTreeWidget->expandItem(itemLeaf);
    }
}

QStringList MdiChild::GetConnectDesc(CAPCONNECTINFO* pcapconnectinfo)
{
    QStringList strStaticList;
    QString strTemp1,strTemp2,strIedDesc;
    SNIFFER_APP* pIED = NULL;
    SNIFFER_PORT* pSnffport = NULL;
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    if(pcapconnectinfo->ncapp_id >= 0)
    {
        pIED = wsSysconfig->I_GET_IED_BYAPPID(pcapconnectinfo->ncapp_id);
        if(pIED != NULL)
        {
            strTemp1 = QString::fromLocal8Bit(pIED->cdesc);
            pSnffport = wsSysconfig->I_GET_SNIFFERPORT(pIED->snifferport_sys_id);
            if(pSnffport != NULL)
                strIedDesc = "[" + QString::number(pIED->snifferport_sys_id,10) + "]" + "-" + strTemp1 + "[" + pSnffport->cportname + "]";
            else
                strIedDesc = "[" + QString::number(pIED->snifferport_sys_id,10) + "]" + strTemp1;
        }
    }
    if(pIED == NULL)
    {
        if(strlen(pcapconnectinfo->csrc1_ip) > 0)
        {
            strTemp1 = pcapconnectinfo->csrc1_ip;
        }
        else
        {
            strTemp1 = pcapconnectinfo->csrc1_mac;
        }
        QByteArray ba = strTemp1.toLatin1();
        pIED = wsSysconfig->I_GET_IED_BYNETIDADDR(ba.data());
        if(pIED != NULL)
        {
            strTemp1 = QString::fromLocal8Bit(pIED->cdesc);
        }
    }

    if(strlen(pcapconnectinfo->csrc2_ip) > 0)
    {
        strTemp2 = pcapconnectinfo->csrc2_ip;
    }
    else
    {
        strTemp2 = pcapconnectinfo->csrc2_mac;
    }
    QByteArray ba = strTemp2.toLatin1();
    pIED = wsSysconfig->I_GET_IED_BYNETIDADDR(ba.data());
    if(pIED != NULL)
    {
        strTemp2 = QString::fromLocal8Bit(pIED->cdesc);
    }

    if(pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_SMV92 || pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_GOOSE)
    {
        if(!strIedDesc.isEmpty())
        {
            strStaticList<<"0X"+QString::number(pcapconnectinfo->ncapp_id,16)+"_"+strIedDesc;
        }
        else
        {
            strStaticList<<"0X"+QString::number(pcapconnectinfo->ncapp_id,16)+"_"+pcapconnectinfo->csrc2_mac;
        }
    }
    else
    {
        strStaticList<<strTemp1+"["+QString::number(pcapconnectinfo->nsrc1packages,10)+"]"+"<-->"+strTemp2+"["+QString::number(pcapconnectinfo->nsrc2packages,10)+"]";
    }

    return strStaticList;
}

QTreeWidgetItem * MdiChild::GetParentByEtherType(int nEtherType)
{
    QStringList childTree;
    QTreeWidgetItem  * itemLeaf;
    if(nEtherType == PROTOAPPTYPE_GOOSE)
    {
        if(itemLeafGs == NULL)
        {
          childTree << QObject::tr("GOOSE");
          itemLeafGs = new QTreeWidgetItem(itemRoot, childTree);
          itemLeafGs->setData(0,Qt::UserRole,QVariant::fromValue(PROTOAPPTYPE_GOOSE));
        }
        itemLeaf = itemLeafGs;
    }
    else if(nEtherType == PROTOAPPTYPE_SMV92)
    {
        if(itemLeafSmv == NULL)
        {
          childTree << QObject::tr("SMV9-2");
          itemLeafSmv = new QTreeWidgetItem(itemRoot, childTree);
          itemLeafSmv->setData(0,Qt::UserRole,QVariant::fromValue(PROTOAPPTYPE_SMV92));
        }
        itemLeaf = itemLeafSmv;
    }
    else if(nEtherType == PROTOAPPTYPE_MMS)
    {
        if(itemLeafMms == NULL)
        {
          childTree << QObject::tr("MMS");
          itemLeafMms = new QTreeWidgetItem(itemRoot, childTree);
          itemLeafMms->setData(0,Qt::UserRole,QVariant::fromValue(PROTOAPPTYPE_MMS));
        }
        itemLeaf = itemLeafMms;
    }
    else if(nEtherType == PROTOAPPTYPE_TIME1588)
    {
        if(itemLeaf1588 == NULL)
        {
          childTree << QObject::tr("1588");
          itemLeaf1588 = new QTreeWidgetItem(itemRoot, childTree);
          itemLeaf1588->setData(0,Qt::UserRole,QVariant::fromValue(PROTOAPPTYPE_TIME1588));
        }
        itemLeaf = itemLeaf1588;
    }
    else if(nEtherType == PROTOAPPTYPE_TCP)
    {
        if(itemLeafTcp == NULL)
        {
          childTree << QObject::tr("TCP");
          itemLeafTcp = new QTreeWidgetItem(itemRoot, childTree);
          itemLeafTcp->setData(0,Qt::UserRole,QVariant::fromValue(PROTOAPPTYPE_TCP));
        }
        itemLeaf = itemLeafTcp;
    }
    else if(nEtherType == PROTOAPPTYPE_ARP)
    {
        if(itemLeafArp == NULL)
        {
          childTree << QObject::tr("ARP_UDP");
          itemLeafArp = new QTreeWidgetItem(itemRoot, childTree);
          itemLeafArp->setData(0,Qt::UserRole,QVariant::fromValue(PROTOAPPTYPE_ARP));
        }
        itemLeaf = itemLeafArp;
    }
    else if(nEtherType == PROTOAPPTYPE_UDP)
    {
        if(itemLeafUdp == NULL)
        {
          childTree << QObject::tr("UDP");
          itemLeafUdp = new QTreeWidgetItem(itemRoot, childTree);
          itemLeafUdp->setData(0,Qt::UserRole,QVariant::fromValue(PROTOAPPTYPE_UDP));
        }
        itemLeaf = itemLeafUdp;
    }
    else
    {
        if(itemLeafOther == NULL)
        {
          childTree << QObject::tr("OTHER");
          itemLeafOther = new QTreeWidgetItem(itemRoot, childTree);
          itemLeafOther->setData(0,Qt::UserRole,QVariant::fromValue(PROTOAPPTYPE_OTHER));
        }
        itemLeaf = itemLeafOther;
    }
    return itemLeaf;
}

void MdiChild::OnMySelectTreeItem(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem *selectItem = item;
    CAPCONNECTINFO *getSelectCapConnectInfo = NULL;
    int selectColumn = column;
    int npcapconnectType = selectItem->data(0,Qt::UserRole).value<int>();
    int nseq = selectItem->data(1,Qt::UserRole).value<int>();
    //qDebug() << npcapconnectType;
    int linkType  = selectItem->data(2,Qt::UserRole).value<int>();
    int nDataShowtype = CAPSHOW_DATATYE_ALL;

    if(npcapconnectType == PROTOAPPTYPE_TOTAL)
        getSelectCapConnectInfo = m_anakit->m_cappackagesmnger.GetPcapconnetInfo(PROTOAPPTYPE_TOTAL);
    else if(npcapconnectType == PROTOAPPTYPE_MMS)
        getSelectCapConnectInfo = m_anakit->m_cappackagesmnger.GetPcapconnetInfo(PROTOAPPTYPE_MMS);
    else if(npcapconnectType == PROTOAPPTYPE_GOOSE)
        getSelectCapConnectInfo = m_anakit->m_cappackagesmnger.GetPcapconnetInfo(PROTOAPPTYPE_GOOSE);
    else if(npcapconnectType == PROTOAPPTYPE_SMV92){
        getSelectCapConnectInfo = m_anakit->m_cappackagesmnger.GetPcapconnetInfo(PROTOAPPTYPE_SMV92);
    }else if(npcapconnectType == PROTOAPPTYPE_TIME1588)
        getSelectCapConnectInfo = m_anakit->m_cappackagesmnger.GetPcapconnetInfo(PROTOAPPTYPE_TIME1588);
    else if(npcapconnectType == PROTOAPPTYPE_ARP)
        getSelectCapConnectInfo = m_anakit->m_cappackagesmnger.GetPcapconnetInfo(PROTOAPPTYPE_ARP);
    else if(npcapconnectType == PROTOAPPTYPE_TCP)
        getSelectCapConnectInfo = m_anakit->m_cappackagesmnger.GetPcapconnetInfo(PROTOAPPTYPE_TCP);
    else if(npcapconnectType == PROTOAPPTYPE_UDP)
        getSelectCapConnectInfo = m_anakit->m_cappackagesmnger.GetPcapconnetInfo(PROTOAPPTYPE_UDP);
    else if(npcapconnectType == PROTOAPPTYPE_OTHER)
        getSelectCapConnectInfo = m_anakit->m_cappackagesmnger.GetPcapconnetInfo(PROTOAPPTYPE_OTHER);
    else
    {
        if(linkType == PROTOAPPTYPE_ERROR)
        {
            nDataShowtype = CAPSHOW_DATATYE_ALARM;

        }
        else if(linkType == PROTOAPPTYPE_EVENT)
        {
            nDataShowtype = CAPSHOW_DATATYE_EVENT;
        }
        else
        {
            ;
        }
        getSelectCapConnectInfo = m_anakit->m_cappackagesmnger.m_mapcapconnectionfo[nseq];
        if(getSelectCapConnectInfo != 0 && getSelectCapConnectInfo->nconnectapptype == PROTOAPPTYPE_SMV92
                && CAPSHOW_DATATYE_ALL == nDataShowtype)
        {
            qDebug() << "OnMySelectTreeItem： begin to show chart...";
            emit wsUpdateWSAnalData(NULL,getSelectCapConnectInfo,nDataShowtype,linkType);
        }
    }
    QObject::connect(this,&MdiChild::wsWriteLinkMsgDetail,linkView,&linkBriefTableView::receiveMsgDetail);

    emit wsWriteLinkMsgDetail(NULL,getSelectCapConnectInfo,nDataShowtype,linkType);

}

