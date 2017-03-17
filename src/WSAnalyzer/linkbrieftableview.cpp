#ifdef OS_WINDOWS
#pragma execution_character_set("UTF-8")
#endif
#include "linkbrieftableview.h"
#include "ui_linkbrieftableview.h"
#include "wssysconfig.h"
#include "AnalyzeOnline_STRUCT.h"
#include "Keywords61850tranformer.h"
#include "LibpCapFileMnger.h"
#include "dlgpktfiltercondition.h"
#include "stores/waveanaldatamodel.h"

#include <QFileDialog>
#include <QQuickWidget>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>


extern CKeywords61850tranformer *keywords61850Ts;
extern CapPackagesMnger ex_m_cappackagesmnger;
detailedMessageView * exDetailedMessageView = NULL;

linkBriefTableView::linkBriefTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::linkBriefTableView),
    m_qwWaveData(0), m_qwWaveAnal(0)
{
    ui->setupUi(this);

    messageView = NULL;
    m_pCapconnectinfoFillter = NULL;
    m_nFindErrPkPos = -1;
    //m_pCapconnectinfoQueryResult = new CAPCONNECTINFO;
    connect(ui->linkBriefTableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(OnMySelectBriefTableItem(int,int)));
    ui->linkBriefTableWidget->horizontalHeader()->setStretchLastSection(true);//设置表格最后列自适应
    ui->linkBriefTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置表格选择一整行
    ui->linkBriefTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);//设置表格可多选
    ui->linkBriefTableWidget->horizontalHeader()->setHighlightSections(false);//设置表头不被选中高亮
    //ui->linkBriefTableWidget->horizontalHeader()->setStyleSheet("QHeaderView {background-color:lblue } QHeaderView::section {background-color:lightblue;color: black;padding-left: 4px;border: 1px solid #6c6c6c;}");
    //"selection-background-color:yellow;"
    ui->linkBriefTableWidget->horizontalHeader()->setStyleSheet("QHeaderView {background-color:lblue } QHeaderView::section {background-color:lightblue;color: black;padding-left: 4px;}");
    ui->linkBriefTableWidget->setStyleSheet("QTableWidget::item:selected { background-color: rgb(255,192,203) }");
    connect(ui->nextPushButton,SIGNAL(clicked(bool)),this, SLOT(OnToolbarNextErrPkt(bool)));
    connect(ui->prePushButton,SIGNAL(clicked(bool)),this, SLOT(OnToolbarPreErrPkt(bool)));
    connect(ui->saveAllPushButton,SIGNAL(clicked(bool)),this, SLOT(OnToolbarExportAll()));
    connect(ui->selectPushButton,SIGNAL(clicked(bool)),this, SLOT(OnToolbarExportSel()));
    connect(ui->quickFindPushButton,SIGNAL(clicked(bool)),this, SLOT(OnToolbarPktFindLink()));
    connect(ui->filterPushButton,SIGNAL(clicked(bool)),this, SLOT(OnToolbarPktFilter()));
    connect(ui->tabMessageListWidget, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));
}

linkBriefTableView::~linkBriefTableView()
{
    delete m_qwWaveData;
    delete ui;
}

/*
 * 生成 [ 0 - nMax ]范围内不重复的数据 nCount 个
 * 注意， nMax 不小于 nCount
 *
 */
QList<int> random(int nMax, int nCount)
{
    QList<int> intList;
    int   i=0, m=0;
    QTime time;
    for(i=0;;)
    {
        if (intList.count() > nCount)
            break;

        int     randn;
        time    = QTime::currentTime();
        qsrand(time.msec()*qrand()*qrand()*qrand()*qrand()*qrand()*qrand());
        randn   = qrand()%nMax;
        m=0;

        while(m<i && intList.at(m)!=randn)
            m++;

        if(m==i)            { intList.append(randn); i++;}
        else if(i==nMax)    break;
        else                continue;
    }

    return intList;
}

/**
 * @brief linkBriefTableView::receiveMsgDetail 接收主界面加载完成PCAP后的通知
 * @param m_cappackagesmnger 数据模型
 * @param getSelectCapConnectInfo 链路结构体
 * @param nDataShowtype 数据类型
 * @param linkType 协议类型
 */
void linkBriefTableView::receiveMsgDetail(CapPackagesMnger *m_cappackagesmnger,CAPCONNECTINFO *getSelectCapConnectInfo,int nDataShowtype,int linkType)
{
    m_nFindErrPkPos = -1;
    if(m_cappackagesmnger != NULL)
    {
        ts_cappackagesmnger = *m_cappackagesmnger;
        RefreshConnectDetail(NULL,CAPSHOW_DATATYE_ALL,PROTOAPPTYPE_TOTAL);
    }
    else
    {
        RefreshConnectDetail(getSelectCapConnectInfo,nDataShowtype,linkType);        
        m_pCapconnectinfoFillter = getSelectCapConnectInfo;
    }
}

/**
 * @brief linkBriefTableView::UpdateWaveAnalDataModel
 * @param pcapconnectinfo
 */
void linkBriefTableView::UpdateWaveAnalDataModel(CapPackagesMnger *m_cappackagesmnger,CAPCONNECTINFO *getSelectCapConnectInfo,int nDataShowtype,int linkType)
{
    if (CAPSHOW_DATATYE_ALL != nDataShowtype)
        return;
    if(0 == getSelectCapConnectInfo)
        return;

    if (0 == m_qwWaveData)
        m_qwWaveData = new WaveAnalDataModel();
    else
        m_qwWaveData->reset();

    qDebug() << "UpdateWaveAnalDataModel： begin to update chart data...";

    //准备采样数据
    MAP_CAPMSGINFO *pMapCapMsgInfo = &getSelectCapConnectInfo->map_capmsginfo;
    int smpCount = pMapCapMsgInfo->size();
    QVector<double> x(smpCount), y(smpCount);
    SMV_INFO_STRUCT *pSMVInfo;
    ASDU_INFO_STRUCT *pAsdu;
    CAPMSGGININFO* pMsgInfo;
    std::map <int, CAPMSGGININFO* >::iterator iter;
    int smp_index=0; //采样点序号
    int chn_index=0; //通道序号
    int chn_count=0; //通道个数

    //获取通道个数
    iter = pMapCapMsgInfo->begin();
    if(iter == pMapCapMsgInfo->end())
        return;
    pMsgInfo = iter->second;
    pSMVInfo = (SMV_INFO_STRUCT*)(pMsgInfo->pparserdstruct);
    chn_count = pSMVInfo->p_asdu_info_struct->n_data_num;

    //画所有通道波形图
    m_qwWaveData->setChannelCount(chn_count);
    for(chn_index = 0; chn_index < chn_count; chn_index++)
    {
        for(smp_index=0, iter = pMapCapMsgInfo->begin(); iter!=pMapCapMsgInfo->end();iter++, smp_index++)
        {

            pMsgInfo = iter->second;
            pSMVInfo = (SMV_INFO_STRUCT*)(pMsgInfo->pparserdstruct);
            pAsdu = pSMVInfo->p_asdu_info_struct;

            x[smp_index] = smp_index;
            qreal y_val = pAsdu->p_smv_data_struct[chn_index].n_value;
            y[smp_index] = pAsdu->p_smv_data_struct[chn_index].n_value/10.0;// + chn_index*2000;
//            if(smp_index > ui->wdtWave->xAxis->range().upper)
//                break;
            if (chn_index == 0)
                m_qwWaveData->xAppend(smp_index);
            m_qwWaveData->yAppend(chn_index, y_val);
        }
    }

    QTabWidget *tabw = ui->tabMessageListWidget;

    // 波形图
    int idx = -1;
    if(!m_qwWaveAnal){
        m_qwWaveAnal = new QQuickWidget();
        m_qwWaveAnal->setObjectName(QStringLiteral("qwWaveAnal"));
        m_qwWaveAnal->setResizeMode(QQuickWidget::SizeRootObjectToView );
        m_qwWaveAnal->setSource(QUrl("qrc:/quick/WSDataAnal.qml"));
        m_qwWaveAnal->rootContext()->setContextProperty("waveModel", m_qwWaveData);
        idx = tabw->addTab(m_qwWaveAnal, QStringLiteral("波形分析"));
    }
    for (int i = 0; i < tabw->count(); i++){
        if(tabw->tabText(i) == QStringLiteral("波形分析")){
            idx = i;
            break;
        }
    }
}

void linkBriefTableView::currentChanged(int index)
{
    messageView->setVisible(index == 0);

    if(m_qwWaveData)
    if (m_qwWaveData->test() == "1")
        m_qwWaveData->setTest("0");
    else
        m_qwWaveData->setTest("1");
}

/**
 * @brief linkBriefTableView::OnToolbarNextErrPkt 下一帧报文
 * @param isClicked
 */
void linkBriefTableView::OnToolbarNextErrPkt(bool isClicked)
{
    if(m_pCapconnectinfoFillter == NULL)
        return;
    int i = 0;
    //根据序号判断顺序
    CAPMSGGININFO* pCapMsgGinInfo = NULL;
    std::map <int, CAPMSGGININFO* >::iterator iter;
    for(iter = m_pCapconnectinfoFillter->map_capmsginfo.begin(); iter != m_pCapconnectinfoFillter->map_capmsginfo.end(); iter ++ )
    {
        pCapMsgGinInfo = iter->second;
        if(pCapMsgGinInfo == NULL)
            continue;
        if(pCapMsgGinInfo->nAppConetentGood == 0 && i > m_nFindErrPkPos && i!=0)
        {
            ui->linkBriefTableWidget->selectRow(i);
            m_nFindErrPkPos = i;
            break;
        }
        i++;
    }
}
/**
 * @brief linkBriefTableView::OnToolbarPreErrPkt 上一帧报文
 * @param isClicked
 */
void linkBriefTableView::OnToolbarPreErrPkt(bool isClicked)
{
    if(m_pCapconnectinfoFillter == NULL)
        return;
    int i = m_pCapconnectinfoFillter->map_capmsginfo.size();
    if(m_nFindErrPkPos == -1)
        m_nFindErrPkPos = m_pCapconnectinfoFillter->map_capmsginfo.size() -1;
    //根据序号判断顺序
    CAPMSGGININFO* pCapMsgGinInfo = NULL;
    std::map <int, CAPMSGGININFO* >::reverse_iterator iter;
    for(iter = m_pCapconnectinfoFillter->map_capmsginfo.rbegin(); iter != m_pCapconnectinfoFillter->map_capmsginfo.rend(); iter ++ )
    {
        i--;
        pCapMsgGinInfo = iter->second;
        if(pCapMsgGinInfo == NULL)
            continue;
        if(pCapMsgGinInfo->nAppConetentGood == 0 && i < m_nFindErrPkPos)
        {
            m_nFindErrPkPos = i;
            ui->linkBriefTableWidget->selectRow(m_nFindErrPkPos);
            break;
        }
    }
}
/**
 * @brief linkBriefTableView::OnToolbarExportAll 导出全部报文
 */
void linkBriefTableView::OnToolbarExportAll()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("选择导出列表文件存放路径"),
            "",
            tr("PCAP Files (*.pcap)"));
    if (!fileName.isNull())
    {
        //fileName是文件名
        //qDebug()<<fileName;
        CLibpCapFileMnger myFileMnger;
        myFileMnger.SaveCapConnetion2File(m_pCapconnectinfoFillter,fileName.toStdString().c_str());
    }
    else
    {
        return;//点的是取消
    }
}
/**
 * @brief linkBriefTableView::OnToolbarExportSel 选中报文另存
 */
void linkBriefTableView::OnToolbarExportSel()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("选择导出列表文件存放路径"),
            "",
            tr("PCAP Files (*.pcap)"));
    if (!fileName.isNull())
    {
        QList<QTableWidgetItem*>items=ui->linkBriefTableWidget->selectedItems();
        int count=items.count();
        CLibpCapFileMnger myFileMnger;
        for(int i=0;i<count;i++)
        {
            int row=ui->linkBriefTableWidget->row(items.at(i));//获取选中的行
            CAPMSGGININFO* pCapMsgGinInfo = GetSelectItemMsg(row);
            myFileMnger.AddPacket2ListBuff(pCapMsgGinInfo);
        }
        myFileMnger.SaveBufferPacketList2File(fileName.toStdString().c_str());
    }
    else
    {
        return;//点的是取消
    }
}
/**
 * @brief linkBriefTableView::OnToolbarPktFindLink 定位链路
 */
void linkBriefTableView::OnToolbarPktFindLink()
{
    QList<QTableWidgetItem*>items=ui->linkBriefTableWidget->selectedItems();
    int row=ui->linkBriefTableWidget->row(items.at(0));//获取选中的行
    CAPMSGGININFO* pCapMsgGinInfo = GetSelectItemMsg(row);
    CAPCONNECTINFO* pCapconnectinfo = (CAPCONNECTINFO*) pCapMsgGinInfo->pcapconnect;
    RefreshLinkBriefTab(pCapconnectinfo,CAPSHOW_DATATYE_ALL);
}
/**
 * @brief linkBriefTableView::OnToolbarPktFilter 报文过滤
 */
void linkBriefTableView::OnToolbarPktFilter()
{
    PACKETQUER_FILTER Query_Filter;
    dlgPktFilterCondition * filter = new dlgPktFilterCondition(this);
    filter->setWindowModality(Qt::ApplicationModal);
    filter->show();
    filter->m_pQuery_Filter = &Query_Filter;
    if(filter->exec() == QDialog::Rejected)
        return;    
    CAPCONNECTINFO * m_pCapconnectinfoQueryResult = new CAPCONNECTINFO;//查询结果连接器
    if(m_nShowDataType == CAPSHOW_DATATYE_ALL)
    {
        if(m_pCapconnectinfoFillter->map_capmsginfo.size()==0)
            return;
        if(ex_m_cappackagesmnger.GetNewFilterConnectionByCondition(m_pCapconnectinfoFillter->map_capmsginfo,&m_pCapconnectinfoQueryResult->map_capmsginfo,&Query_Filter)==RES_SUCCESS)
        {
            if(m_pCapconnectinfoQueryResult->map_capmsginfo.size()==0)
            {
                ui->linkBriefTableWidget->setRowCount(0);
                ui->linkBriefTableWidget->clearContents();
                return;
            }
            RefreshLinkBriefTab(m_pCapconnectinfoQueryResult,CAPSHOW_DATATYE_ALL);            
        }
    }
    if(m_nShowDataType == CAPSHOW_DATATYE_ALARM)
    {
        if(m_pCapconnectinfoFillter->map_capmsginfo_error.size()==0)
            return;
        if(ex_m_cappackagesmnger.GetNewFilterConnectionByCondition(m_pCapconnectinfoFillter->map_capmsginfo_error,&m_pCapconnectinfoQueryResult->map_capmsginfo_error,&Query_Filter)==RES_SUCCESS)
        {
            if(m_pCapconnectinfoQueryResult->map_capmsginfo_error.size()==0)
            {
                ui->linkBriefTableWidget->setRowCount(0);
                ui->linkBriefTableWidget->clearContents();
                return;
            }
            RefreshLinkBriefTab(m_pCapconnectinfoQueryResult,CAPSHOW_DATATYE_ALARM);
        }
    }
    if(m_nShowDataType == CAPSHOW_DATATYE_EVENT)
    {
        if(m_pCapconnectinfoFillter->map_capmsginfo_event.size()==0)
            return;
        if(ex_m_cappackagesmnger.GetNewFilterConnectionByCondition(m_pCapconnectinfoFillter->map_capmsginfo_event,&m_pCapconnectinfoQueryResult->map_capmsginfo_event,&Query_Filter)==RES_SUCCESS)
        {
            if(m_pCapconnectinfoQueryResult->map_capmsginfo_event.size()==0)
            {
                ui->linkBriefTableWidget->setRowCount(0);
                ui->linkBriefTableWidget->clearContents();
                return;
            }
            RefreshLinkBriefTab(m_pCapconnectinfoQueryResult,CAPSHOW_DATATYE_EVENT);
        }
    }
}
/**
 * @brief linkBriefTableView::GetSelectItemMsg 获取选中信息包的信息
 * @param nItemIndex 行号索引
 * @return
 */
CAPMSGGININFO* linkBriefTableView::GetSelectItemMsg(int nItemIndex)
{
    CAPMSGGININFO* pCapMsgGinInfo = NULL;
    if(nItemIndex < 0 )
        return pCapMsgGinInfo;
    pCapMsgGinInfo = m_pmsgginifoSS[nItemIndex];
    return pCapMsgGinInfo;
}
/**
 * @brief linkBriefTableView::OnMySelectBriefTableItem 选中列表，选择某行后发送信号通知详细报文界面更新数据
 * @param row 行号
 * @param column 列号
 */
void linkBriefTableView::OnMySelectBriefTableItem(int row, int column)
{
    //ui->messageTreeWidget->clear();
    //WsWriteMsgDetail(m_pmsgginifoSS[row]);
    //FillParseredMsg_All(m_pmsgginifoSS[row]);
    QObject::connect(this,&linkBriefTableView::wsWriteMsgDetail,messageView,&detailedMessageView::receiveMsgDetail);
    emit wsWriteMsgDetail(m_pmsgginifoSS[row]);
}
/**
 * @brief linkBriefTableView::RefreshConnectDetail 刷新连接
 * @param pcapconnectinfo
 * @param nDataShowtype
 * @param npcapconnectType
 */
void linkBriefTableView::RefreshConnectDetail(CAPCONNECTINFO *pcapconnectinfo, int nDataShowtype, int npcapconnectType)
{
    if(pcapconnectinfo == NULL)
    {
        pcapconnectinfo = ts_cappackagesmnger.GetPcapconnetInfo(npcapconnectType);
    }
    if(pcapconnectinfo == NULL)
        return;
    RefreshLinkBriefTab(pcapconnectinfo,nDataShowtype);
}
/**
 * @brief linkBriefTableView::RefreshLinkBriefTab 刷新连接列表界面
 * @param pcapconnectinfo
 * @param nDataShowtype
 */
void linkBriefTableView::RefreshLinkBriefTab(CAPCONNECTINFO *pcapconnectinfo, int nDataShowtype)
{
    QString strTemp;
    int npktsize = 0;
    if(nDataShowtype == CAPSHOW_DATATYE_ALL)
    {
        npktsize = pcapconnectinfo->map_capmsginfo.size();
    }
    if(nDataShowtype == CAPSHOW_DATATYE_ALARM)
    {
        npktsize = pcapconnectinfo->map_capmsginfo_error.size();
    }
    if(nDataShowtype == CAPSHOW_DATATYE_EVENT)
    {
        npktsize = pcapconnectinfo->map_capmsginfo_event.size();
    }
    m_nShowDataType = nDataShowtype;
    if(pcapconnectinfo != NULL)
    {
        if(pcapconnectinfo->nusetype == 1)
        {
            strTemp = "报文列表:全部报文_"+QString::number(npktsize,10)+"帧";
        }
        else
        {
            strTemp = "报文列表:" + GetLinkDesc(pcapconnectinfo) + "_" + QString::number(npktsize,10) + "帧";
        }
        ui->tabMessageListWidget->setTabText(0,strTemp);
    }
    else
    {
        strTemp = "报文列表";
        ui->tabMessageListWidget->setWindowTitle(strTemp);
    }
    FillTGridTreeControl(pcapconnectinfo,nDataShowtype);


}
/**
 * @brief linkBriefTableView::FillTGridTreeControl 根据数据类型填充连接数据
 * @param pcapconnectinfo 连接数据结构体
 * @param nDataShowtype 数据类型
 */
void linkBriefTableView::FillTGridTreeControl(CAPCONNECTINFO *pcapconnectinfo, int nDataShowtype)
{
    /*
    QHeaderView* headerView = ui->linkBriefTableWidget->verticalHeader();
    headerView->setHidden(true);//隐藏行编号
    */
    m_pCapconnectinfoFillter = pcapconnectinfo;
    if(nDataShowtype == CAPSHOW_DATATYE_ALL)
    {
        FillTGridTree(pcapconnectinfo->map_capmsginfo);
    }
    if(nDataShowtype == CAPSHOW_DATATYE_ALARM)
    {
        FillTGridTree(pcapconnectinfo->map_capmsginfo_error);
    }
    if(nDataShowtype == CAPSHOW_DATATYE_EVENT)
    {
        FillTGridTree(pcapconnectinfo->map_capmsginfo_event);
    }
}
/**
 * @brief linkBriefTableView::FillTGridTree 填充表格
 * @param m_pmsgginifo 信息包结构体
 */
void linkBriefTableView::FillTGridTree(MAP_CAPMSGINFO m_pmsgginifo)
{
    m_pmsgginifoSS = m_pmsgginifo;
    std::map<int , CAPMSGGININFO*>::iterator iter;
    int i = 0;
    CAPMSGGININFO *pmsgginifo;
    int nMode = 1;
    if(m_pCapconnectinfoFillter->nusetype == 1)//全部报文
        nMode = 0;
    for(iter = m_pmsgginifo.begin();iter != m_pmsgginifo.end();iter++)
    {
        pmsgginifo = iter->second;
        if(pmsgginifo == NULL)
            continue;
        QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(pmsgginifo->nseq,10));
        ui->linkBriefTableWidget->setRowCount(m_pmsgginifo.size());
        item1->setTextAlignment(Qt::AlignCenter);
        ui->linkBriefTableWidget->setItem(i,0,item1);
        QTableWidgetItem *item2 = new QTableWidgetItem(GetTimeTxt(pmsgginifo,nMode));
        item2->setTextAlignment(Qt::AlignCenter);
        ui->linkBriefTableWidget->setItem(i,1,item2);
        QTableWidgetItem *item3 = new QTableWidgetItem(GetListContent_Cloumn3(pmsgginifo));
        item3->setTextAlignment(Qt::AlignCenter);
        ui->linkBriefTableWidget->setItem(i,2,item3);
        QTableWidgetItem *item4 = new QTableWidgetItem(GetListContent_Cloumn4(pmsgginifo));
        item4->setTextAlignment(Qt::AlignCenter);
        ui->linkBriefTableWidget->setItem(i,3,item4);
        QTableWidgetItem *item5 = new QTableWidgetItem(GetListContent_Cloumn5(pmsgginifo));
        item5->setTextAlignment(Qt::AlignCenter);
        ui->linkBriefTableWidget->setItem(i,4,item5);
        QTableWidgetItem *item6 = new QTableWidgetItem(GetListContent_Cloumn6(pmsgginifo));
        item6->setTextAlignment(Qt::AlignCenter);
        ui->linkBriefTableWidget->setItem(i,5,item6);
        QTableWidgetItem *item7 = new QTableWidgetItem(GetListContent_Cloumn7(pmsgginifo));
        item7->setTextAlignment(Qt::AlignCenter);
        ui->linkBriefTableWidget->setItem(i,6,item7);
        i++;
    }
    ui->linkBriefTableWidget->resizeColumnsToContents();//根据内容自动调整所有列的列宽
    ui->linkBriefTableWidget->selectRow(0);
    ui->linkBriefTableWidget->horizontalHeader()->setStretchLastSection(true);//设置表格最后列自适应

    if(messageView == NULL)
    {
        messageView = new detailedMessageView();
        exDetailedMessageView = messageView;
        ui->splitter->addWidget(messageView);
        ui->splitter->setStretchFactor(0,7);
        ui->splitter->setStretchFactor(1,3);
    }
    QObject::connect(this,&linkBriefTableView::wsWriteMsgDetail,messageView,&detailedMessageView::receiveMsgDetail);
    emit wsWriteMsgDetail(m_pmsgginifo[0]);
}
/**
 * @brief linkBriefTableView::FillTGirdTreForTimeStyles 时间格式设置
 */
void linkBriefTableView::FillTGirdTreForTimeStyles()
{
    std::map<int , CAPMSGGININFO*>::iterator iter;
    int i = 0;
    CAPMSGGININFO *pmsgginifo;
    for(iter = m_pmsgginifoSS.begin();iter != m_pmsgginifoSS.end();iter++)
    {
        pmsgginifo = iter->second;
        if(pmsgginifo == NULL)
            continue;
        QTableWidgetItem *item2 = new QTableWidgetItem(GetTimeTxt(pmsgginifo,0));
        item2->setTextAlignment(Qt::AlignCenter);
        ui->linkBriefTableWidget->setItem(i,1,item2);
        i++;
    }
    ui->linkBriefTableWidget->resizeColumnsToContents();
    ui->linkBriefTableWidget->horizontalHeader()->setStretchLastSection(true);//设置表格最后列自适应
}
/**
 * @brief linkBriefTableView::GetTimeTxt 按系统配置获取时间格式
 * @param pCapMsgGinInfo 信息包结构体
 * @param nMode 1 全数据 0 指定链路
 * @return
 */
QString linkBriefTableView::GetTimeTxt(CAPMSGGININFO* pCapMsgGinInfo,int nMode)
{
    if(pCapMsgGinInfo == NULL)
        return "";
    QString strTime;
    float fTime = 0.0f;
    int nTime = 0.0;
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();

    if(wsSysconfig->m_wsShowStyle.nDataViewTimeStyle == 0)//长时间格式
    {
        time_t ttime = (time_t)pCapMsgGinInfo->nseconds_utc_tmstamp;
        struct tm *local;
        local=localtime(&ttime);
        char buf[80];
        strftime(buf,80,"%Y-%m-%d %H:%M:%S.",local);
        strTime = buf + QString::number(pCapMsgGinInfo->nus_tmstamp,10);
    }
    else if(wsSysconfig->m_wsShowStyle.nDataViewTimeStyle == 1)//短时间格式
    {
        time_t ttime = (time_t)pCapMsgGinInfo->nseconds_utc_tmstamp;
        struct tm *local;
        local=localtime(&ttime);
        char buf[80];
        strftime(buf,80,"%H:%M:%S.",local);
        strTime = buf + QString::number(pCapMsgGinInfo->nus_tmstamp,10);
    }
    else if(wsSysconfig->m_wsShowStyle.nDataViewTimeStyle == 4)//1970后的秒.us
    {
        strTime = QString::number(pCapMsgGinInfo->nseconds_utc_tmstamp,10) +"."+ QString::number(pCapMsgGinInfo->nus_tmstamp,10);
    }
    else if(wsSysconfig->m_wsShowStyle.nDataViewTimeStyle == 2)//与第一帧时间差格式
    {
        if(nMode == 0)//全数据下和上一帧报文相对时间获取不同
        {
            fTime = pCapMsgGinInfo->ftime_relative ;
        }
        else
        {
            fTime = pCapMsgGinInfo->ftime_relative_capconnect ;
        }
        char buf[10];
        fTime *= 1000;
        sprintf(buf, "%0.3f", fTime);
        strTime = buf;

    }
    else if(wsSysconfig->m_wsShowStyle.nDataViewTimeStyle == 3)//与上一帧时间差格式
    {
        if(nMode == 0)
        {
            fTime = pCapMsgGinInfo->ftime_delta;
        }
        else
        {
            fTime = pCapMsgGinInfo->ftime_delta_capconnect;
        }
        fTime *= 1000000;
        nTime = (int)fTime;
        if(fTime - nTime > 0.5)
            nTime++;
        strTime = QString::number(nTime,10);
    }
    return strTime;
}
/**
 * @brief linkBriefTableView::GetListContent_Cloumn3 得到第三列数据内容--发送端
 * @param pCapMsgGinInfo 信息包结构体
 * @return
 */
QString linkBriefTableView::GetListContent_Cloumn3(CAPMSGGININFO* pCapMsgGinInfo)
{
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    SNIFFER_APP* pIed = NULL;
    QString strText;
    if(pCapMsgGinInfo->napp_id > 0)
    {
        pIed = wsSysconfig->I_GET_IED_BYAPPID(pCapMsgGinInfo->napp_id);
    }
    if(pIed == NULL)
    {
        if(strlen(pCapMsgGinInfo->csrc_ip) != 0)//默认填写ip地址
        {
            strText = pCapMsgGinInfo->csrc_ip;
        }
        else //无ip填写mac地址
        {
            strText = pCapMsgGinInfo->csrc_mac;
        }
        QByteArray ba = strText.toLatin1();
        pIed = wsSysconfig->I_GET_IED_BYNETIDADDR(ba.data());
        if(pIed != NULL)
        {
            strText = QString::fromLocal8Bit(pIed->cdesc);
        }
    }
    else
    {
        strText = QString::fromLocal8Bit(pIed->cdesc);
    }
    //更新显示
    if(pCapMsgGinInfo->pap_analyzed_info != NULL && pCapMsgGinInfo->napptype == PROTOAPPTYPE_MMS)//MMS报文
    {
        AP_ACSI_INFO* pAp_Acsi_Info = (AP_ACSI_INFO*) pCapMsgGinInfo->pap_analyzed_info;
        if(strlen(pAp_Acsi_Info->source))
        {
            strText = pAp_Acsi_Info->source;
        }
    }
    return strText;
}
/**
 * @brief linkBriefTableView::GetListContent_Cloumn4  得到第四列数据内容--接收端
 * @param pCapMsgGinInfo 信息包结构体
 * @return
 */
QString linkBriefTableView::GetListContent_Cloumn4(CAPMSGGININFO *pCapMsgGinInfo)
{
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    SNIFFER_APP* pIed = NULL;
    QString strText;
    if(strlen(pCapMsgGinInfo->cdst_ip) != 0)//默认填写ip地址
    {
        strText = pCapMsgGinInfo->cdst_ip;
    }
    else //无ip填写mac地址
    {
        strText = pCapMsgGinInfo->cdst_mac;
    }
    QByteArray ba = strText.toLatin1();
    pIed = wsSysconfig->I_GET_IED_BYNETIDADDR(ba.data());
    if(pIed != NULL)
    {
        strText = QString::fromLocal8Bit(pIed->cdesc);
    }
    //更新显示
    if(pCapMsgGinInfo->pap_analyzed_info != NULL && pCapMsgGinInfo->napptype == PROTOAPPTYPE_MMS)//MMS报文
    {
        AP_ACSI_INFO* pAp_Acsi_Info = (AP_ACSI_INFO*) pCapMsgGinInfo->pap_analyzed_info;
        //接收端
        if(strlen(pAp_Acsi_Info->dest))
        {
            strText = pAp_Acsi_Info->dest;
        }
    }
    return strText;
}
/**
 * @brief linkBriefTableView::GetListContent_Cloumn5 得到第五列数据内容--报文类型
 * @param pCapMsgGinInfo 信息包结构体
 * @return
 */
QString linkBriefTableView::GetListContent_Cloumn5(CAPMSGGININFO *pCapMsgGinInfo)
{
    QString strText;
    if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_OTHER)//ethtype_unkown
    {
            strText ="其它";
    }
    else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_GOOSE)
    {
        strText ="GOOSE";
    }
    else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_SMV92)
    {
        strText = "SV 9-2";
    }
    else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_MMS)
    {
        strText ="MMS";
    }
    else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_COTP)
    {
        strText = "COTP";
    }
    else if(pCapMsgGinInfo->napptype == ETHER_TYPE_TPKT)
    {
        strText = "TPKT";
    }
    else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_ARP)
    {
        strText ="ARP";
    }
    else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_TCP)
    {
        strText ="TCP";
    }
    else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_TIME1588)
    {
        strText ="PTPv2";
    }
    else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_UDP)
    {
        strText ="UDP";
    }
    else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_IGMP)
    {
        strText ="IGMP";
    }
    else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_LLDP)
    {
        strText ="LLDP";
    }
    else  if(pCapMsgGinInfo->napptype == ETHER_TYPE_DROP)
    {
        strText ="丢弃";
    }
    else
    {
        strText ="其它";
    }
    return strText;
}
/**
 * @brief linkBriefTableView::GetListContent_Cloumn6 得到第六列数据内容--报文内容
 * @param pCapMsgGinInfo 信息包结构体
 * @return
 */
QString linkBriefTableView::GetListContent_Cloumn6(CAPMSGGININFO *pCapMsgGinInfo)
{
    QString strText;
    strText = pCapMsgGinInfo->ccontent;
    return strText;
}
/**
 * @brief linkBriefTableView::GetListContent_Cloumn7 得到第七列数据内容--错误内容
 * @param pCapMsgGinInfo 信息包结构体
 * @return
 */
QString linkBriefTableView::GetListContent_Cloumn7(CAPMSGGININFO *pCapMsgGinInfo)
{
    QString strText,strTempContent,strTemp;
    if(pCapMsgGinInfo->nHeadChkgood != 1)
    {
        strText = "数据包头校验和错误";
    }
    if(pCapMsgGinInfo->pap_analyzed_info != NULL)
    {
        if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_GOOSE)//GOOSE报文
        {
            strTempContent = GetGooseErrDesc(pCapMsgGinInfo);
        }
        else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_SMV92)//SMV报文
        {
            strTempContent = GetSmvErrDesc(pCapMsgGinInfo);
        }
        else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_MMS)//MMS报文
        {
            AP_ACSI_INFO* pAp_Acsi_Info = (AP_ACSI_INFO*) pCapMsgGinInfo->pap_analyzed_info;
            strTempContent.clear();
            if(pAp_Acsi_Info->err_code != NULL)
            {
                if(*pAp_Acsi_Info->err_code != 9999)
                {
                    pCapMsgGinInfo->nAppConetentGood = 0;//错误报文
                    strTempContent = QString::fromLocal8Bit(keywords61850Ts->Get_Acsi_Error_Desc(*pAp_Acsi_Info->err_code).c_str());
                }
            }
        }/*
        else if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_TIME1588)
        {
            strTempContent = GetPtpErrDesc(pCapMsgGinInfo);
        }*/
        if(!strTempContent.isEmpty())
            strText = strTempContent;
    }
    //1588 记录事件戳
   /* if(pCapMsgGinInfo->napptype == PROTOAPPTYPE_TIME1588)
    {
        PTP_INFO_STRUCT* p_ptp_info_struct = (PTP_INFO_STRUCT*) pCapMsgGinInfo->pparserdstruct;
        __int64 time_s = 0;
        UINT time_ns = 0;
        char sTime[256]="";
        if(p_ptp_info_struct != NULL)
        {
            if(pApp->m_piec61850Analyzer->m_ptpv2Analyzer->I_GET_STRING_INFO(p_ptp_info_struct,0,sTime) == 0)
                strText.Format("%s|%s",(const char*)strText,sTime);
        }
    }*/
    return strText;
}
/**
 * @brief linkBriefTableView::GetSmvErrDesc 获取SMV描述
 * @param pCapMsgGinInfo 信息包结构体
 * @return
 */
QString linkBriefTableView::GetSmvErrDesc(CAPMSGGININFO* pCapMsgGinInfo)
{
    QString strText,strTempContent,strTemp;
    int nerrorCode;
    MESSAGE_ERROR_INFO_ARRAY_STRUCT* pArrayStruct;

    pArrayStruct = (MESSAGE_ERROR_INFO_ARRAY_STRUCT*) pCapMsgGinInfo->pap_analyzed_info;
    strTempContent.clear();
    for(int i = 0; i< pArrayStruct->n_msg_err; i++)
    {
        for(int j = 0; j < pArrayStruct->p_msg_err[i].n_num_errcode; j++)//逐行读取错误信号
        {
            nerrorCode      =  pArrayStruct->p_msg_err[i].p_errcode[j];
            if(nerrorCode == 16 )//链路正常告警，关闭
                continue;
            if(strTempContent.isEmpty())
                strTempContent += QString::fromLocal8Bit(keywords61850Ts->Get_SMV_Error_ChineseDesc(nerrorCode).c_str());
            else
                strTempContent = strTempContent+ "/" + QString::fromLocal8Bit(keywords61850Ts->Get_SMV_Error_ChineseDesc(nerrorCode).c_str());
        }
        //
        for(int j = 0; j < pArrayStruct->p_msg_err[i].n_num_asduerr; j++)//统计asdu中的错误
        {
            for(int k = 0; k < pArrayStruct->p_msg_err[i].p_asduerr[j].n_num_errcode; k++)
            {
                nerrorCode = pArrayStruct->p_msg_err[i].p_asduerr[j].p_errcode[k];
                if(nerrorCode == 16 )//链路正常告警，关闭
                    continue;
                if(strTempContent.isEmpty())
                    strTempContent = QString::fromLocal8Bit(keywords61850Ts->Get_SMV_Error_ChineseDesc(nerrorCode).c_str());
                else
                    strTempContent = strTempContent + "/" + QString::fromLocal8Bit(keywords61850Ts->Get_SMV_Error_ChineseDesc(nerrorCode).c_str());
            }
            for(int l = 0; l < pArrayStruct->p_msg_err[i].p_asduerr[j].n_num_daerr; l++)
            {
                for(int ll = 0; ll < pArrayStruct->p_msg_err[i].p_asduerr[j].p_daerr[l].n_num_errcode; ll ++)
                {
                    nerrorCode = pArrayStruct->p_msg_err[i].p_asduerr[j].p_daerr[l].p_errcode[ll];
                    strTemp = QString::number(pArrayStruct->p_msg_err[i].p_asduerr[j].p_daerr[l].n_da_seq+1,10) + keywords61850Ts->Get_SMV_Error_ChineseDesc(nerrorCode).c_str();
                    if(strTempContent.isEmpty())
                    {
                        strTempContent = strTemp;
                    }
                    else
                    {
                        strTempContent = strTempContent + "/" + strTemp;
                    }
                }
            }
        }
    }
    return strTempContent;
}
/**
 * @brief linkBriefTableView::GetGooseErrDesc 获取Goose描述
 * @param pCapMsgGinInfo 信息包结构体
 * @return
 */
QString linkBriefTableView::GetGooseErrDesc(CAPMSGGININFO* pCapMsgGinInfo)
{
    QString strText,strTempContent,strTemp,strGooseError;
    int nerrorCode;
    MESSAGE_ERROR_INFO_ARRAY_STRUCT* pArrayStruct;
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    APP_CHANNEL_RELATION* pRelation = NULL;
    GOOSE_INFO_STRUCT* pGooseInfoStruct = (GOOSE_INFO_STRUCT*) pCapMsgGinInfo->pparserdstruct;
    SNIFFER_APP * pIed = wsSysconfig->I_GET_IED_BYAPPID(pGooseInfoStruct->n_app_id);
    if(pIed != NULL)//获取通道描述
    {
        pRelation = wsSysconfig->I_GET_APP_4Y_CFGINFO(pIed->nsys_appsqe);//获取4Y配置
    }
    pArrayStruct = (MESSAGE_ERROR_INFO_ARRAY_STRUCT*) pCapMsgGinInfo->pap_analyzed_info;
    strTempContent.clear();
    for(int i = 0; i< pArrayStruct->n_msg_err; i++)
    {
        for(int j = 0; j < pArrayStruct->p_msg_err[i].n_num_errcode; j++)//统计外围错误项
        {
            nerrorCode      =  pArrayStruct->p_msg_err[i].p_errcode[j];
            strTempContent = strTempContent + QString::fromLocal8Bit(keywords61850Ts->Get_GOOSE_Error_ChineseDesc(nerrorCode).c_str()) + "/";
        }
        if(!strTempContent.isEmpty())
            strTempContent = strTempContent.left(strTempContent.length()-1);//去除多余的/
        //状态变位以及通道数据类型不一致处理
        for(int j = 0; j < pArrayStruct->p_msg_err[i].n_num_asduerr; j++)//统计详细数据项错误
        {
            if(pArrayStruct->p_msg_err[i].p_asduerr[j].n_num_errcode <= 0)
                continue;
            nerrorCode = pArrayStruct->p_msg_err[i].p_asduerr[j].p_errcode[0];
            if(nerrorCode == 24)//状态变位
            {
                strTemp = "通道_"+QString::number(pArrayStruct->p_msg_err[i].p_asduerr[j].n_asdu_no+1,10);
                strGooseError += strTemp ;
            }
            else//目前只有通道数据类型不一致
            {
                strTemp = QString::number(pArrayStruct->p_msg_err->p_asduerr[j].n_asdu_no+1,10) + "-" + keywords61850Ts->Get_GOOSE_Error_ChineseDesc(nerrorCode).c_str();
                if(strTempContent.isEmpty())
                {
                    strTempContent = strTemp;
                }
                else
                {
                    strTempContent = strTempContent + "...";
                    break;
                }
            }
        }
    }
    if(!strGooseError.isEmpty())
    {
        strGooseError = strGooseError.left(strGooseError.length()-1);
        strTempContent = strTempContent  + "/" + "状态变位:" +strGooseError ;
    }
    return strTempContent;
}
/**
 * @brief linkBriefTableView::GetLinkDesc 获取连接描述
 * @param pcapconnectinfo 链路结构体
 * @return
 */
QString linkBriefTableView::GetLinkDesc(CAPCONNECTINFO *pcapconnectinfo)
{
    QString linkDesc1,linkDesc2;
    QString strTemp1,strTemp2;
    SNIFFER_APP* pIED = NULL;
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    if(pcapconnectinfo->ncapp_id >= 0)
    {
        pIED = wsSysconfig->I_GET_IED_BYAPPID(pcapconnectinfo->ncapp_id);
        if(pIED != NULL)
        {
            strTemp1 = QString::fromLocal8Bit(pIED->cdesc) + "(" + QString::fromLocal8Bit(pIED->ciedname) + ")" ;
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
            strTemp1 = QString::fromLocal8Bit(pIED->cdesc) + "(" + QString::fromLocal8Bit(pIED->ciedname) + ")";
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
        strTemp2 = QString::fromLocal8Bit(pIED->cdesc) + "(" + QString::fromLocal8Bit(pIED->ciedname) + ")";
    }
    if(pcapconnectinfo->ncapp_id >= 0)
    {
        if(pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_SMV92)
        {
            linkDesc1 = "SMV 0x" + QString::number(pcapconnectinfo->ncapp_id,16);
        }
        else if(pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_GOOSE)
        {
            linkDesc1 = "GOOSE 0x" + QString::number(pcapconnectinfo->ncapp_id,16);
        }
    }
    else
    {
        if(pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_MMS)
        {
            linkDesc1 = "MMS_" + QString::number(pcapconnectinfo->nseq,10);
        }
    }
    if(pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_SMV92)
    {
        linkDesc2 = strTemp1;
    }
    else
    {
        linkDesc2 = strTemp1 + "<-->" + strTemp2;
    }
    return linkDesc1 +":"+ linkDesc2;

}
