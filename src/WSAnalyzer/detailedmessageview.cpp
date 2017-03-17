#ifdef OS_WINDOWS
#pragma execution_character_set("UTF-8")
#endif
#include "detailedmessageview.h"
#include "ui_detailedmessageview.h"
#include "ScanDissectPacketer.h"
#include "AnalyzeOnline_STRUCT.h"
#include "wssysconfig.h"
#include "Keywords61850tranformer.h"
#include "CapTransformer.h"
#include "tsscdparser_define.h"
#include "TsScdParserWrapper.h"
#include <QFile>
#include <QDebug>

#include <QQuickWidget>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>


extern CScanDissectPacketer      *g_ScanDissectPacketer;      //用于数据包的解析
extern CCapTransformer * g_capTransformer;  //cap文件生成的txt文件解析器
extern CKeywords61850tranformer *keywords61850Ts; //61850字典
detailedMessageView::detailedMessageView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::detailedMessageView)
{
    ui->setupUi(this);
//    setupMoreUi();

    hexEdit = NULL;
    connect(ui->messageTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(OnMySelectTreeItem(QTreeWidgetItem*,int)));
}

detailedMessageView::~detailedMessageView()
{
    delete ui;
}

void detailedMessageView::setupMoreUi()
{
    QTabWidget *tabw = ui->tabWidget;

//    m_qwTimeDisper = new QQuickWidget();
//    m_qwTimeDisper->setObjectName(QStringLiteral("qwTimeDisper"));
//    m_qwTimeDisper->setResizeMode(QQuickWidget::SizeRootObjectToView );
//    m_qwTimeDisper->setSource(QUrl("qrc:/quick/WSTimeDisperAnal.qml"));

//    tabw->addTab(m_qwTimeDisper, QStringLiteral("离散度分析"));

    m_qwHarmonic = new QQuickWidget();
    m_qwHarmonic->setObjectName(QStringLiteral("qwHarmonic"));
    m_qwHarmonic->setResizeMode(QQuickWidget::SizeRootObjectToView );
    m_qwHarmonic->setSource(QUrl("qrc:/quick/WSHarmonicAnal.qml"));

    tabw->addTab(m_qwHarmonic, QStringLiteral("谐波分析"));

    m_qwVector = new QQuickWidget();
    m_qwVector->setObjectName(QStringLiteral("qwVector"));
    m_qwVector->setResizeMode(QQuickWidget::SizeRootObjectToView );
    m_qwVector->setSource(QUrl("qrc:/quick/WSSequenceAnal.qml"));

    tabw->addTab(m_qwVector, QStringLiteral("向量分析"));

    m_qwSequence = new QQuickWidget();
    m_qwSequence->setObjectName(QStringLiteral("qwSequence"));
    m_qwSequence->setResizeMode(QQuickWidget::SizeRootObjectToView );
    m_qwSequence->setSource(QUrl("qrc:/quick/WSSequenceAnal.qml"));

    tabw->addTab(m_qwSequence, QStringLiteral("序分量分析"));
}

/**
 * @brief detailedMessageView::receiveMsgDetail 接收报文列表加载完成通知
 * @param pMsgGinInfo 报文帧信息结构体
 */
void detailedMessageView::receiveMsgDetail(CAPMSGGININFO *pMsgGinInfo)
{
    //qDebug() << QString::number(pMsgGinInfo->nseq);
    ui->messageTreeWidget->clear();
    WsWriteMsgDetail(pMsgGinInfo);
    FillParseredMsg_All(pMsgGinInfo);
    QHeaderView *pHeader=ui->messageTreeWidget->header();
    pHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
    pHeader->setStretchLastSection(false);
}
/**
 * @brief detailedMessageView::WsWriteMsgDetail 调用接口解析数据包，填入报文概要信息
 * @param pMsgGinInfo 报文帧信息结构体
 */
void detailedMessageView::WsWriteMsgDetail(CAPMSGGININFO *pMsgGinInfo)
{
    if(!pMsgGinInfo)
        return;
    g_ScanDissectPacketer->I_XJ_CLEANUP_DISSECT();
    if(pMsgGinInfo->map_cotplist.size() > 1)
    {
        g_ScanDissectPacketer->I_XJ_DISSECT_MUL_PACKET(pMsgGinInfo,0);
    }
    else
    {
        g_ScanDissectPacketer->I_XJ_DISSECT_PACKET(pMsgGinInfo,0);
    }
    FillParseredMsg_PacketDesc(pMsgGinInfo);
    ui->tabWidget->setCurrentIndex(0);
    //ui->listView->setVisible(true);
    //mmsTab = ui->tabWidget->findChildren<QWidget*>().at(1);
    //ui->tabWidget->removeTab(1);

    //MMS报文解析
    if(pMsgGinInfo->napptype == IEC61850_ETHER_TYPE_MMS || pMsgGinInfo->napptype == ETHER_TYPE_COTP || pMsgGinInfo->napptype == ETHER_TYPE_TPKT)//MMS报文
    {
        //ui->tabWidget->addTab(mmsTab,"MMS解析");
        ui->tabWidget->setCurrentIndex(1);
        FillMsgDetail(pMsgGinInfo);//填充窗口
    }
}

/**
 * @brief detailedMessageView::FillMsgDetail 填充MMS报文控制
 * @param pMsgGinInfo 报文帧信息结构体
 */
void detailedMessageView::FillMsgDetail(CAPMSGGININFO *pMsgGinInfo)
{
    //MMS报文解析
    if(pMsgGinInfo->napptype == IEC61850_ETHER_TYPE_MMS || pMsgGinInfo->napptype == ETHER_TYPE_COTP || pMsgGinInfo->napptype == ETHER_TYPE_TPKT)//MMS报文
    {
        if(pMsgGinInfo->pparserdstruct == NULL)//制作MMS报文,格式错误报文不显示
        {
            g_capTransformer->I_XJ_PKT_STRUCT_MAKE_MMS_INFO_STRUCT(pMsgGinInfo,FALSE);
        }
        FillParseredMsg_MmsStruct(pMsgGinInfo,FALSE);
        if(pMsgGinInfo->pap_anlyzed_info_connect != NULL)//关联报文
        {
            g_capTransformer->I_XJ_PKT_STRUCT_MAKE_MMS_INFO_STRUCT((CAPMSGGININFO*)pMsgGinInfo->pap_anlyzed_info_connect,FALSE);
            FillParseredMsg_MmsStruct((CAPMSGGININFO*)pMsgGinInfo->pap_anlyzed_info_connect,TRUE);
        }
    }
    /*
    QHexEdit *hexEdit = new QHexEdit;
    hexEdit->setData(QByteArray(pMsgGinInfo->csourceinfo,pMsgGinInfo->nsourceinfo_length));
    hexEdit->setSelection(10);
    ui->splitter->addWidget(hexEdit);
    */
}
/**
 * @brief detailedMessageView::OnMySelectTreeItem 选中树节点监听,控制16进制界面响应
 * @param item 选中的元素
 * @param column
 */
void detailedMessageView::OnMySelectTreeItem(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem *selectItem = item;
    XJ_PROTO_NODE *xjNode = selectItem->data(1,Qt::UserRole).value<XJ_PROTO_NODE *>();
    if(!xjNode)
        return;
    hexEdit->setSelectionInit(xjNode->start_pos);
    hexEdit->setSelection((xjNode->start_pos+xjNode->length)*2);
    hexEdit->viewport()->update();
    //qDebug()<<"start "+ QString::number(xjNode->start_pos);
    //qDebug()<<"length "+QString::number(xjNode->length);
}
/**
 * @brief detailedMessageView::HexContrl2TreeControl 16进制界面控制树控件响应
 * @param nPosition 选中位置
 */
void detailedMessageView::HexContrl2TreeControl(int nPosition)
{
    if(nPosition < 0 || nPosition > m_pMsgGinInfo->ncap_len)
        return;
    XJ_PROTO_NODE* pNode = NULL;
    for(int i = 0; i < m_pMsgGinInfo->pxj_dissect_pkt->prototreelist_size;i++)
    {
        pNode = GetProtoNodeByHexPosition(nPosition,&m_pMsgGinInfo->pxj_dissect_pkt->pprototreelist[i]);
        if(pNode != NULL)
            break;
    }
    hexEdit->setSelectionInit(pNode->start_pos);
    hexEdit->setSelection((pNode->start_pos+pNode->length)*2);
    hexEdit->viewport()->update();
    if(pNode)
    {
        ui->messageTreeWidget->setCurrentItem((QTreeWidgetItem*)pNode->pkt_usrview);
    }
}
/**
 * @brief detailedMessageView::GetProtoNodeByHexPosition 获取报文节点信息
 * @param nPosition 选中位置
 * @param pParentNode 报文节点信息
 * @return
 */
XJ_PROTO_NODE * detailedMessageView::GetProtoNodeByHexPosition(int nPosition,XJ_PROTO_NODE* pParentNode)
{
    XJ_PROTO_NODE* pNode = NULL;
    for(int i = 0; i < pParentNode->childlist_size; i++)
    {
        pNode = GetProtoNodeByHexPosition(nPosition,&pParentNode->pchildlist[i]);
        if(pNode != NULL)
            break;
    }
    if(pNode == NULL)
    {
        if(pParentNode->start_pos <= nPosition && pParentNode->length + pParentNode->start_pos-1 >= nPosition)
            pNode = pParentNode;
    }
    return pNode;
}
/**
 * @brief detailedMessageView::FillParseredMsg_PacketDesc 填充详细报文概要信息
 * @param pMsgGinInfo 报文帧信息结构体
 */
void detailedMessageView::FillParseredMsg_PacketDesc(CAPMSGGININFO *pMsgGinInfo)
{
    QString strTime;
    time_t ttime = (time_t)pMsgGinInfo->nseconds_utc_tmstamp;
    struct tm *local;
    local=localtime(&ttime);
    char buf[80];
    strftime(buf,80,"%Y-%m-%d %H:%M:%S.",local);
    strTime = buf + QString::number(pMsgGinInfo->nus_tmstamp,10);
    QStringList rootTree;
        rootTree << "概要信息, 时间戳："+strTime+" 长度：" + QString::number(pMsgGinInfo->ncap_len);//树形统计头根节点
    itemDetailedRoot = new QTreeWidgetItem(ui->messageTreeWidget,rootTree);
    ui->messageTreeWidget->expandItem(itemDetailedRoot);
}
/**
 * @brief detailedMessageView::FillParseredMsg_All 填充各种报文类型
 * @param pMsgGinInfo 报文帧信息结构体
 */
void detailedMessageView::FillParseredMsg_All(CAPMSGGININFO *pMsgGinInfo)
{
    if(pMsgGinInfo == NULL)
        return;
    m_pMsgGinInfo = pMsgGinInfo;
    for(int i = 0; i < pMsgGinInfo->pxj_dissect_pkt->prototreelist_size; i++)
    {
            if(i != pMsgGinInfo->pxj_dissect_pkt->prototreelist_app_seq)
            {
                TreeCtrlOutputXjPrototree(&pMsgGinInfo->pxj_dissect_pkt->pprototreelist[i],itemDetailedRoot,TRUE);
            }
            else//特别展示的应用层
            {
                switch(pMsgGinInfo->napptype)
                {
                case PROTOAPPTYPE_SMV92:
                    FillParseredMsg_SMV92Struct(pMsgGinInfo);
                    break;
                case PROTOAPPTYPE_GOOSE:
                    FillParseredMsg_GooseStruct(pMsgGinInfo);
                    break;
                default:
                    TreeCtrlOutputXjPrototree(&pMsgGinInfo->pxj_dissect_pkt->pprototreelist[i],itemDetailedRoot,TRUE);
                    break;
                }
            }
    }
    if(hexEdit==NULL)
    {
        hexEdit = new QHexEdit;
        ui->splitter->addWidget(hexEdit);
    }
    int lenHex=pMsgGinInfo->nsourceinfo_length;
    if (pMsgGinInfo->ncap_len-pMsgGinInfo->nlen>7)//尾部信息
    {
        lenHex-=8;
    }
    hexEdit->setData(QByteArray(pMsgGinInfo->csourceinfo+16,lenHex-16));
    //hexEdit->setSelection(10);

}
/**
 * @brief detailedMessageView::TreeCtrlOutputXjPrototree 填充详细报文
 * @param pprototreelist 报文节点信息
 * @param itemSuper 树节点
 * @param bTopLevel
 * @param bExpand
 */
void detailedMessageView::TreeCtrlOutputXjPrototree(XJ_PROTO_NODE *pprototreelist,QTreeWidgetItem *itemSuper, BOOL bTopLevel, BOOL bExpand)
{
    QString strContent;
    QStringList rootTree;
    //制作描述
    if(bTopLevel)
    {
        strContent = pprototreelist->node_name;
    }
    else
    {
        if(pprototreelist->node_name_detail != NULL)
        {
            strContent = pprototreelist->node_name_detail;
            strContent.append(": ");
            strContent.append(pprototreelist->node_desc);
        }
        else
        {
            strContent = pprototreelist->node_name;
        }
    }
    rootTree << strContent;
    QTreeWidgetItem *item;
    if(bTopLevel)
        item = new QTreeWidgetItem(itemSuper,rootTree);
    else
        item= new QTreeWidgetItem(itemSuper,rootTree);
    item->setData(1,Qt::UserRole,QVariant::fromValue(pprototreelist));
    pprototreelist->pkt_usrview = (void*) item;//记忆本节点的位置
    if(bExpand)//展开节点
    {
       ;
    }
    for(int i = 0; i < pprototreelist->childlist_size; i++)
    {
        TreeCtrlOutputXjPrototree(&pprototreelist->pchildlist[i],item,FALSE,bExpand);
    }
}
/**
 * @brief detailedMessageView::GetLinkDesc 获取连接描述
 * @param pMsgGinInfo 报文帧信息结构体
 * @return
 */
QString detailedMessageView::GetLinkDesc(CAPMSGGININFO *pMsgGinInfo)
{
    //基本信息
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    MMS_INFO_STRUCT*  pMmsInfoStruct = (MMS_INFO_STRUCT* )pMsgGinInfo->pparserdstruct;
    SNIFFER_APP* pIed = wsSysconfig->I_GET_IED_BYNETIDADDR(pMsgGinInfo->csrc_ip);
    QString strTempName,strsrcIed,strDstIed,strTemp1,strTemp2;
    if(pIed != NULL)
    {
        strsrcIed.sprintf("%s",QString::fromLocal8Bit(pIed->cdesc).toStdString().c_str());
    }
    pIed = wsSysconfig->I_GET_IED_BYNETIDADDR(pMsgGinInfo->cdst_ip);
    if(pIed != NULL)
    {
        strDstIed.sprintf("%s",QString::fromLocal8Bit(pIed->cdesc).toStdString().c_str());
    }
    if(!strsrcIed.isEmpty())
    {
        strTemp1.sprintf("%s(%s)",strsrcIed.toStdString().c_str(),pMsgGinInfo->csrc_ip);
    }
    else
    {
        strTemp1.sprintf("%s",pMsgGinInfo->csrc_ip);
    }
    if(!strDstIed.isEmpty())
    {
        strTemp2.sprintf("%s(%s)",strDstIed.toStdString().c_str(),pMsgGinInfo->cdst_ip);
    }
    else
    {
        strTemp2.sprintf("%s",pMsgGinInfo->cdst_ip);
    }
    strTempName.sprintf("%s---->%s %s[%s]",
        strTemp1.toStdString().c_str(),strTemp2.toStdString().c_str(),pMsgGinInfo->ccontent,pMmsInfoStruct->c_service_type);
    return strTempName;
}
/**
 * @brief detailedMessageView::FillParseredMsg_MmsStruct 填充MMS报文结构
 * @param pMsgGinInfo 报文帧信息结构体
 * @param bInsertConnect 关联报文判断
 */
void detailedMessageView::FillParseredMsg_MmsStruct(CAPMSGGININFO *pMsgGinInfo, BOOL bInsertConnect)
{
    if(!bInsertConnect)//清空
    {
        ui->mmsMessageTreeWidget->clear();
    }
    if(pMsgGinInfo->pparserdstruct == NULL)
        return;
    MMS_INFO_STRUCT*  pMmsInfoStruct = (MMS_INFO_STRUCT* )pMsgGinInfo->pparserdstruct;
    //数据输出
    QString strTempName;
    strTempName = GetLinkDesc(pMsgGinInfo);

    if(bInsertConnect)
        strTempName = "关联报文："+ strTempName;

    QTreeWidgetItem *hItem,*hsubItem,*childeItem;
    if(!bInsertConnect)
        hItem = new QTreeWidgetItem(ui->mmsMessageTreeWidget,QStringList(strTempName));//在报文描述之后
    else
        hItem = new QTreeWidgetItem(ui->mmsMessageTreeWidget,QStringList(strTempName));//在报文描述之后
    //InvokeID
    strTempName.sprintf("invokeID:[%d]",pMmsInfoStruct->n_invoke_id);
    hsubItem = new QTreeWidgetItem(hItem,QStringList(strTempName));
    //根据服务类型填充
    //PDU类型为confirmed-ErrorPDU，则填充服务最终结果和错误原因
    if(strstr(pMmsInfoStruct->c_pdu_type,"ErrorPDU"))
    {
        hsubItem = new QTreeWidgetItem(hItem,QStringList("错误(ErrorPDU)"));
        strTempName.sprintf("错误类别:%s",keywords61850Ts->Get_MMS_SRV_ERROR_CLASS_ChineseDesc(pMmsInfoStruct->struct_mms_service_error.n_error_class).c_str());
        childeItem = new QTreeWidgetItem(hsubItem,QStringList(strTempName));
        strTempName.sprintf("错误号:%d",pMmsInfoStruct->struct_mms_service_error.n_error_code);
        childeItem = new QTreeWidgetItem(hsubItem,QStringList(strTempName));
        ui->mmsMessageTreeWidget->expandItem(hsubItem);
        ui->mmsMessageTreeWidget->expandItem(hItem);
        return;
    }
    else if(strstr(pMmsInfoStruct->c_pdu_type,"rejectPDU"))//拒绝
    {
        hsubItem = new QTreeWidgetItem(hItem,QStringList("拒绝服务(rejectPDU)"));
        strTempName.sprintf("发起的invokeid:[%d]",pMmsInfoStruct->struct_mms_service_reject.n_originalInvoikeID);
        childeItem = new QTreeWidgetItem(hsubItem,QStringList(strTempName));

        strTempName.sprintf("拒绝报文类型:%s",keywords61850Ts->GET_REJECT_PDU_ChineseDesc(pMmsInfoStruct->struct_mms_service_reject.n_reject_pdu).c_str());
        childeItem = new QTreeWidgetItem(hsubItem,QStringList(strTempName));

        strTempName.sprintf("拒绝号：[%d]",pMmsInfoStruct->struct_mms_service_reject.n_reject_reason_code);
        ui->mmsMessageTreeWidget->expandItem(hsubItem);
        ui->mmsMessageTreeWidget->expandItem(hItem);
        return;
    }
    //根据不同的服务输出不同的内容
    //文件目录服务
    if (strstr(pMmsInfoStruct->c_service_type,"fileDirectory"))
    {
        hsubItem = new QTreeWidgetItem(hItem,QStringList("文件服务"));
        FillParseredMsg_MmsStruct_FILEDIRECTORY(pMmsInfoStruct,hsubItem);
        ui->mmsMessageTreeWidget->expandItem(hsubItem);
    }
    else if(NULL!=strstr(pMmsInfoStruct->c_service_type,"initiate"))//启动
    {
        hsubItem = new QTreeWidgetItem(hItem,QStringList("初始化"));
        FillParseredMsg_MmsStruct_Init(pMmsInfoStruct,hsubItem);
        ui->mmsMessageTreeWidget->expandItem(hsubItem);
    }
    else if(NULL!=strstr(pMmsInfoStruct->c_service_type,"informationReport"))//信息报告
    {
        hsubItem = new QTreeWidgetItem(hItem,QStringList("信息报告"));
        FillParseredMsg_MmsStruct_Report(pMmsInfoStruct,hsubItem);
        ui->mmsMessageTreeWidget->expandItem(hsubItem);
    }
    else
    {
        FillParseredMsg_MmsStruct_Detail(pMmsInfoStruct,hItem);//详细填充
    }
    //详细数据
    if(!bInsertConnect)
        ui->mmsMessageTreeWidget->expandItem(hItem);

}
/**
 * @brief detailedMessageView::FillParseredMsg_SMV92Struct 填充SMV报文结构
 * @param pMsgGinInfo 报文帧信息结构体
 */
void detailedMessageView::FillParseredMsg_SMV92Struct(CAPMSGGININFO *pMsgGinInfo)
{
    if(pMsgGinInfo->pparserdstruct == NULL)
        return;
    //填写第一层
    XJ_PROTO_NODE * pproto,*pprotoChild,*pprotoChildChild;
    int nChildSize,nChildChildSize;
    pproto = &pMsgGinInfo->pxj_dissect_pkt->pprototreelist[pMsgGinInfo->pxj_dissect_pkt->prototreelist_app_seq];//取到对应层号
    if(pproto == NULL)
        return;
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    QStringList strTempName;
    QString strtemp;
    QTreeWidgetItem *hItem,*hSubItem,*hsubsubItem,*hItemSeqAsdu,*hItemAsduData;
    //写入根
    SMV_INFO_STRUCT* pSmvInfoStruct = (SMV_INFO_STRUCT* )pMsgGinInfo->pparserdstruct;
    SNIFFER_APP * pIed = wsSysconfig->I_GET_IED_BYAPPID(pSmvInfoStruct->n_app_id);
    APP_CHANNEL_RELATION* pRelation = NULL;
    BOOL bWriteData = FALSE;
    int nconfigchannelsize =-1;//配置的通道数
    if(pIed != NULL)
    {
        strtemp = QString::fromLocal8Bit(pIed->cdesc)+"("+QString::fromLocal8Bit(pIed->ciedname)+")";
        pRelation = wsSysconfig->I_GET_APP_4Y_CFGINFO(pIed->nsys_appsqe);//获取4Y配置
        nconfigchannelsize = pRelation->n_yc_channel_num;
    }
    else
    {
        strtemp = pMsgGinInfo->cdst_mac;
    }
    //写入顶层子节点

    strTempName<<"9-2采样值 发送装置:"+QString(strtemp)+" APPID:0x"+QString::number(pSmvInfoStruct->n_app_id,16);
    hItem = new QTreeWidgetItem(itemDetailedRoot,strTempName);
    hItem->setData(1,Qt::UserRole,QVariant::fromValue(pproto));
    pproto->pkt_usrview = (void*) hItem;//记忆本节点的位置
    QTreeWidgetItem *hItemRoot = hItem;//记住根节点方便展开
    //写入第一层子节点
    nChildSize = pproto->childlist_size;
    if(nChildSize < 5 || pproto->pchildlist == NULL)
        return;
    //APPID
    strTempName.clear();
    strTempName<<QString(pproto->pchildlist[0].node_name_detail)+": "+QString(pproto->pchildlist[0].node_desc);
    hSubItem = new QTreeWidgetItem(hItem,strTempName);
    hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(&pproto->pchildlist[0]));
    pproto->pchildlist[0].pkt_usrview = (void*) hSubItem;//记忆本节点的位置
    //报文长度
    strTempName.clear();
    strTempName<<"报文长度: "+QString(pproto->pchildlist[1].node_desc);
    hSubItem = new QTreeWidgetItem(hItem,strTempName);
    hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(&pproto->pchildlist[1]));
    pproto->pchildlist[1].pkt_usrview = (void*) hSubItem;//记忆本节点的位置
    //备用1
    strTempName.clear();
    strTempName<<"备用1: "+QString(pproto->pchildlist[2].node_desc);
    hSubItem = new QTreeWidgetItem(hItem,strTempName);
    hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(&pproto->pchildlist[2]));
    pproto->pchildlist[2].pkt_usrview = (void*) hSubItem;//记忆本节点的位置
    //备用2
    strTempName.clear();
    strTempName<<"备用2: "+QString(pproto->pchildlist[2].node_desc);
    hSubItem = new QTreeWidgetItem(hItem,strTempName);
    hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(&pproto->pchildlist[3]));
    pproto->pchildlist[3].pkt_usrview = (void*) hSubItem;//记忆本节点的位置
    //协议数据单元
    strTempName.clear();
    strTempName<<"协议数据单元(PDU)";
    hItemSeqAsdu = new QTreeWidgetItem(hItem,strTempName);
    hItemSeqAsdu->setData(1,Qt::UserRole,QVariant::fromValue(&pproto->pchildlist[4]));
    pproto->pchildlist[4].pkt_usrview = (void*) hItemSeqAsdu;//记忆本节点的位置
    //写入第二层结构--协议数据单元--PDU
    nChildSize = pproto->pchildlist[4].childlist_size;
    pprotoChild = pproto->pchildlist[4].pchildlist;
    if(nChildSize < 2 || pproto->pchildlist[4].pchildlist == NULL)
        return;
    pprotoChild++;
    nChildSize =pprotoChild->childlist_size;
    pprotoChild = pprotoChild->pchildlist;
    for(int i = 0; i < nChildSize; i++)
    {
        strTempName.clear();
        strTempName<<"应用服务单元(ASDU)["+QString::number(i+1)+"]";
        hItemAsduData = new QTreeWidgetItem(hItemSeqAsdu,strTempName);//此处父节点hItemSeqAsdu即为 line709中加载的应用服务单元(ASDU)：%s
        hItemAsduData->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
        pprotoChild->pkt_usrview = (void*) hItemAsduData;//记忆本节点的位置
        //开始写入第四层
        nChildChildSize = pprotoChild->childlist_size;
        pprotoChildChild = pprotoChild->pchildlist;
        bWriteData = FALSE;
        for(int j = 0; j < nChildChildSize; j++)//应用服务单元的数据
        {
            strTempName.clear();
            if(strcmp(pprotoChildChild->node_name,"svID") ==0)
            {
                if(pIed != NULL)
                {
                    strTempName<<"采样标识(svID): "+QString(pprotoChildChild->node_desc)+" 配置为:"+QString(pIed->csvid);
                    if(strcmp(pprotoChildChild->node_desc,pIed->csvid) != 0)
                        pprotoChildChild->errortype = 2;
                }
                else
                {
                    strTempName<<"采样标识(svID): "+QString(pprotoChildChild->node_desc);
                }
                hSubItem = new QTreeWidgetItem(hItemAsduData,strTempName);
                hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChildChild));
                pprotoChildChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
            }
            else if(strcmp(pprotoChildChild->node_name,"smpCnt") == 0)
            {
                strTempName<<"采样记数(smpCnt): "+QString(pprotoChildChild->node_desc);
                hSubItem = new QTreeWidgetItem(hItemAsduData,strTempName);
                hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChildChild));
                pprotoChildChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
            }
            else if(strcmp(pprotoChildChild->node_name,"smpSynch") ==0)
            {
                strTempName<<"同步标识(smpSynch): "+QString(pprotoChildChild->node_desc);
                hSubItem = new QTreeWidgetItem(hItemAsduData,strTempName);
                hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChildChild));
                pprotoChildChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
                if(strcmp(pprotoChildChild->node_desc,"local (1)") != 0)
                    pprotoChildChild->errortype = 2;
            }
            else if(strcmp(pprotoChildChild->node_name,"confRef")== 0)
            {
                if(pIed != NULL)
                {
                    if(atoi(pprotoChildChild->node_desc) != pIed->nconfrev)
                    {
                        strTempName<<"版本号(confRef): "+QString(pprotoChildChild->node_desc)+" 配置为:"+QString::number(pIed->nconfrev);
                        pprotoChildChild->errortype = 2;
                    }
                    else
                    {
                        strTempName<<"版本号(confRef): "+QString(pprotoChildChild->node_desc);
                    }
                }
                else
                {
                    strTempName<<"版本号(confRef): "+QString(pprotoChildChild->node_desc);
                }
                hSubItem = new QTreeWidgetItem(hItemAsduData,strTempName);
                hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChildChild));
                pprotoChildChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
            }
            else if(strcmp(pprotoChildChild->node_name,"Text item") == 0)//详细数据
            {
                bWriteData = TRUE;
                int nDataSize = pprotoChildChild->childlist_size/2;
                XJ_PROTO_NODE * pprotoData = pprotoChildChild->pchildlist;
                XJ_PROTO_NODE * pprotoQuarlity;
                if(nconfigchannelsize != -1)
                {
                    strTempName<<"采样通道数据: 共"+QString::number(nDataSize)+"通道  配置为"+QString::number(nconfigchannelsize)+"通道";
                    if(nDataSize != nconfigchannelsize)//错误标识
                        pprotoChildChild->errortype = 2;
                }
                else
                {
                    strTempName<<"采样通道数据: 共"+QString::number(nDataSize)+"通道";
                }
                hSubItem = new QTreeWidgetItem(hItemAsduData,strTempName);
                hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChildChild));
                pprotoChildChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
                for(int k = 0; k< nDataSize; k ++)//开始写入数据
                {
                    QString changeSuper;
                    strTempName.clear();
                    pprotoQuarlity = pprotoData + 1;//品质指针
                    //写入数据
                    changeSuper.sprintf("数据%02d:%15s  品质: %s",k+1,pprotoData->node_desc,pprotoQuarlity->node_desc);
                    strTempName<<changeSuper;
                    if(pRelation != NULL)
                    {
                        if(k < pRelation->n_yc_channel_num)
                        {
                            strTempName<<"数据"+QString::number(k+1)+"-"+QString(pRelation->pyc_channel_list[k]->cdesc)+":"+QString(pprotoData->node_desc)+
                                         "  品质: "+QString(pprotoQuarlity->node_desc);
                            strTempName<<changeSuper;
                        }
                    }
                    hsubsubItem = new QTreeWidgetItem(hSubItem,strTempName);
                    hsubsubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoData));
                    pprotoData->pkt_usrview = (void*) hsubsubItem;//记忆本节点的位置

                    //写入品质
                    XJ_PROTO_NODE * pprotoDataQy = pprotoQuarlity->pchildlist;
                    for(int kk = 0; kk < pprotoQuarlity->childlist_size; kk ++)//品质解析
                    {
                        QString change;
                        strTempName.clear();
                        if(strcmp(pprotoDataQy->node_name,"validity")== 0)
                        {
                            change.sprintf("%s(有效性): %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                            if(strcmp(pprotoDataQy->node_desc,"good (0x00000000)") != 0)
                            {
                                pprotoDataQy->errortype = 2;//错误
                                pprotoData->errortype = 2;
                            }
                        }
                        else if(strcmp(pprotoDataQy->node_name , "overflow")== 0)
                        {
                            change.sprintf("%s(溢出): %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                        }
                        else if(strcmp(pprotoDataQy->node_name , "out of range")==0)
                        {
                            change.sprintf("%s(超值域): %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                        }
                        else if(strcmp(pprotoDataQy->node_name ,"bad reference")== 0)
                        {
                            change.sprintf("%s(坏基准值): %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                        }
                        else if(strcmp(pprotoDataQy->node_name , "oscillatory")==0)
                        {
                            change.sprintf("%s(抖动): %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                        }
                        else if(strcmp(pprotoDataQy->node_name , "failure")== 0)
                        {
                            change.sprintf("%s(故障): %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                        }
                        else if(strcmp(pprotoDataQy->node_name , "old data")==0)
                        {
                            change.sprintf("%s(老数据): %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                        }
                        else if(strcmp(pprotoDataQy->node_name , "inconsistent")==0)
                        {
                            change.sprintf("%s(不一致): %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                        }
                        else if(strcmp(pprotoDataQy->node_name , "inaccurate")==0)
                        {
                            change.sprintf("%s(不精确): %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                        }
                        else if(strcmp(pprotoDataQy->node_name , "source")==0)
                        {
                            change.sprintf("%s(数据源): %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                            if(strcmp(pprotoDataQy->node_desc,"process (0x00000000)") != 0)
                            {
                                pprotoDataQy->errortype = 2;//错误
                                pprotoData->errortype = 2;
                            }
                        }
                        else if(strcmp(pprotoDataQy->node_name , "test")==0)
                        {
                            change.sprintf("%s(检修标志): %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                            if(strcmp(pprotoDataQy->node_desc,"False") != 0)
                            {
                                pprotoDataQy->errortype = 2;//错误
                                pprotoData->errortype = 2;
                            }
                        }
                        else if(strcmp(pprotoDataQy->node_name , "operator blocked")==0)
                        {
                            change.sprintf("%s(操作闭锁): %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                            if(strcmp(pprotoDataQy->node_desc,"False") != 0)
                            {
                                pprotoDataQy->errortype = 2;//错误
                                pprotoData->errortype = 2;
                            }
                        }
                        else if(strcmp(pprotoDataQy->node_name , "derived")==0)
                        {
                            change.sprintf("%s: %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                        }
                        else
                        {
                            change.sprintf("%s: %10s",pprotoDataQy->node_name_detail,pprotoDataQy->node_desc);
                            strTempName<<change;
                        }
                        hItem = new QTreeWidgetItem(hsubsubItem,strTempName);
                        hItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoDataQy));
                        pprotoDataQy->pkt_usrview = (void*) hItem;//记忆本节点的位置
                        pprotoDataQy ++;
                    }
                    pprotoData++;//跳过2个节点
                    pprotoData++;
                }
            }
            else if(strcmp(pprotoChildChild->node_name , "BER Error")==0)//其它
            {
                ;
            }
            else
            {
                strTempName<<QString(pprotoChildChild->node_name)+": "+QString(pprotoChildChild->node_desc);
                hSubItem = new QTreeWidgetItem(hItemAsduData,strTempName);
                hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChildChild));
                pprotoChildChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
            }
            ui->messageTreeWidget->expandItem(hItemAsduData);
            pprotoChildChild++;
        }//应用服务单元的数据
        //数据弥补，9-2报文数据
        if(bWriteData == FALSE && i < pSmvInfoStruct->n_asdu_num)//未写入
        {
            strTempName.clear();
            strTempName<<"采样通道数据: 共"+QString::number(pSmvInfoStruct->p_asdu_info_struct[i].n_data_num)+"通道";
            hSubItem = new QTreeWidgetItem(hItemAsduData,strTempName);
            for(int k = 0; k< pSmvInfoStruct->p_asdu_info_struct[i].n_data_num; k ++)//开始写入数据
            {
                //写入数据
                strTempName.clear();
                QString str;
                str.sprintf("数据%02d:%15d 品质: 0x%02x%02x%02x%02x",k+1,pSmvInfoStruct->p_asdu_info_struct[i].p_smv_data_struct[k].n_value,
                            (unsigned char)pSmvInfoStruct->p_asdu_info_struct[i].p_smv_data_struct[k].c_quality[0],(unsigned char)pSmvInfoStruct->p_asdu_info_struct[i].p_smv_data_struct[k].c_quality[1],
                            (unsigned char)pSmvInfoStruct->p_asdu_info_struct[i].p_smv_data_struct[k].c_quality[2],(unsigned char)pSmvInfoStruct->p_asdu_info_struct[i].p_smv_data_struct[k].c_quality[3]);
                strTempName<<str;
                if(pRelation != NULL)
                {
                    if(k < pRelation->n_yc_channel_num)
                    {
                        // fRatio=1.0f;
                        char szUnit[20]={'\0'};
                        if (pRelation->pyc_channel_list[k]->cunits[0]=='A' || pRelation->pyc_channel_list[k]->cunits[0]=='V')
                        {
                            //fRatio = pRelation->pyc_channel_list[k]->fratio;
                            #ifdef OS_WINDOWS
                            _snprintf_s(szUnit,19,"(精度:%.3f%s)",pRelation->pyc_channel_list[k]->fratio,pRelation->pyc_channel_list[k]->cunits);
                            #else
                            snprintf(szUnit,19,"(精度:%.3f%s)",pRelation->pyc_channel_list[k]->fratio,pRelation->pyc_channel_list[k]->cunits);
                            #endif
                        }
                        strTempName.clear();
                        QString str;
                        str.sprintf("数据%02d-%s:%15d%s 品质: 0x%02x%02x%02x%02x",k+1,QString::fromLocal8Bit(pRelation->pyc_channel_list[k]->cdesc).toStdString().c_str(),pSmvInfoStruct->p_asdu_info_struct[i].p_smv_data_struct[k].n_value,szUnit,
                            (unsigned char)pSmvInfoStruct->p_asdu_info_struct[i].p_smv_data_struct[k].c_quality[0],(unsigned char)pSmvInfoStruct->p_asdu_info_struct[i].p_smv_data_struct[k].c_quality[1],
                            (unsigned char)pSmvInfoStruct->p_asdu_info_struct[i].p_smv_data_struct[k].c_quality[2],(unsigned char)pSmvInfoStruct->p_asdu_info_struct[i].p_smv_data_struct[k].c_quality[3]);
                        strTempName<<str;
                    }
                }
                hsubsubItem = new QTreeWidgetItem(hSubItem,strTempName);
                //写入品质内容

            }
        }
        pprotoChild++;
    }
    ui->messageTreeWidget->expandItem(hItemRoot);
    ui->messageTreeWidget->expandItem(hItemSeqAsdu);
}
/**
 * @brief MdiChild::FillParseredMsg_GooseStruct 填充goose结构
 * @param pMsgGinInfo 报文帧信息结构体
 */
void detailedMessageView::FillParseredMsg_GooseStruct(CAPMSGGININFO *pMsgGinInfo)
{
    if(pMsgGinInfo->pparserdstruct == NULL)
        return;
    //填写第一层
    XJ_PROTO_NODE * pproto,*pprotoChild,*pprotoChildChild;
    int nChildSize;
    pproto = &pMsgGinInfo->pxj_dissect_pkt->pprototreelist[pMsgGinInfo->pxj_dissect_pkt->prototreelist_app_seq];//取到对应层号
    if(pproto == NULL)
        return;
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    QString strtemp;
    QStringList strTempName;
    QTreeWidgetItem  *hItem,*hSubItem,*hsubsubItem,*hItemSeqAsdu,*hItemAsduData;
    //写入根
    GOOSE_INFO_STRUCT* pGooseInfoStruct = (GOOSE_INFO_STRUCT*) pMsgGinInfo->pparserdstruct;
    SNIFFER_APP * pIed = wsSysconfig->I_GET_IED_BYAPPID(pGooseInfoStruct->n_app_id);
    APP_CHANNEL_RELATION* pRelation = NULL;
    int nconfigchannelsize =-1;//配置的通道数
    if(pIed != NULL)
    {
        strtemp.sprintf("%s(%s)",QString::fromLocal8Bit(pIed->cdesc).toStdString().c_str(),pIed->ciedname);
        pRelation = wsSysconfig->I_GET_APP_4Y_CFGINFO(pIed->nsys_appsqe);//获取4Y配置
        nconfigchannelsize = pRelation->n_yx_channel_num;
    }
    else
    {
        strtemp = pMsgGinInfo->cdst_mac;
    }
    //写入顶层子节点
    strTempName << "IEC 61850 GOOSE 发送装置:" + strtemp + " APPID:0x" + QString::number(pGooseInfoStruct->n_app_id);
    hItem = new QTreeWidgetItem(itemDetailedRoot,strTempName);
    hItem->setData(1,Qt::UserRole,QVariant::fromValue(pproto));
    //SetItemData(hItem,(DWORD_PTR)pproto);
   pproto->pkt_usrview = (void*) hItem;//记忆本节点的位置
    //写入第一层子节点
    nChildSize = pproto->childlist_size;
    if(nChildSize < 5 || pproto->pchildlist == NULL)
        return;
    //APPID
    strTempName.clear();
    strTempName << QString(pproto->pchildlist[0].node_name_detail) + ": " + QString(pproto->pchildlist[0].node_desc);
    hSubItem = new QTreeWidgetItem(hItem,strTempName);
    hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(&pproto->pchildlist[0]));
    pproto->pchildlist[0].pkt_usrview = (void*) hSubItem;//记忆本节点的位置
    //报文长度
    strTempName.clear();
    strTempName <<"报文长度: "+QString(pproto->pchildlist[1].node_desc);
    hSubItem = new QTreeWidgetItem(hItem,strTempName);
    hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(&pproto->pchildlist[1]));
    pproto->pchildlist[1].pkt_usrview = (void*) hSubItem;//记忆本节点的位置
    //备用1
    strTempName.clear();
    strTempName << "备用1: " + QString(pproto->pchildlist[2].node_desc);
    hSubItem = new QTreeWidgetItem(hItem,strTempName);
    hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(&pproto->pchildlist[2]));
    pproto->pchildlist[2].pkt_usrview = (void*) hSubItem;//记忆本节点的位置
    //备用2
    strTempName.clear();
    strTempName << "备用2: " + QString(pproto->pchildlist[3].node_desc);
    hSubItem = new QTreeWidgetItem(hItem,strTempName);
    hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(&pproto->pchildlist[3]));
    pproto->pchildlist[3].pkt_usrview = (void*) hSubItem;//记忆本节点的位置
    //协议数据单元
    strTempName.clear();
    strTempName << "协议数据单元(PDU)";
    hItemSeqAsdu = new QTreeWidgetItem(hItem,strTempName);
    hItemSeqAsdu->setData(1,Qt::UserRole,QVariant::fromValue(&pproto->pchildlist[4]));
//	pproto->pchildlist[4].pkt_usrview = (void*) hItemSeqAsdu;//记忆本节点的位置
    //写入第二层结构--协议数据单元--PDU
    nChildSize  = pproto->pchildlist[4].childlist_size;
    pprotoChild = pproto->pchildlist[4].pchildlist;
    for(int i = 0; i < nChildSize; i ++)
    {
        strTempName.clear();
        if(strcmp(pprotoChild->node_name,"gocbRef") ==0)
        {
            strTempName<< "GOOSE控制块参引(gocbRef): "+QString(pprotoChild->node_desc);
            hSubItem = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
        }
        else if(strcmp(pprotoChild->node_name,"timeAllowedtoLive") ==0)
        {
            strTempName << "报文生存时间(timeAllowedtoLive): "+QString(pprotoChild->node_desc);
            hSubItem = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
        }
        else if(strcmp(pprotoChild->node_name,"datSet") ==0)
        {
            strTempName<<"数据集参引(DatSetRef): "+QString(pprotoChild->node_desc);
            if(pIed != NULL)
            {
                if(strcmp(pprotoChild->node_desc,pIed->cdsname) != 0)
                {
                    strTempName<<"数据集参引(DatSetRef): "+QString(pprotoChild->node_desc)+" 配置为："+QString(pIed->cdsname);
                    pprotoChild->errortype = 2;
                }
            }
            hSubItem = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
        }
        else if(strcmp(pprotoChild->node_name,"goID") ==0)
        {
            strTempName<<"GOOSE标识(goID): "+QString(pprotoChild->node_desc);
            if(pIed != NULL)
            {
                if(strcmp(pprotoChild->node_desc,pIed->cgoid) != 0)
                {
                    strTempName<<"GOOSE标识(goID): "+QString(pprotoChild->node_desc)+" 配置为："+QString(pIed->cgoid);
                    pprotoChild->errortype = 2;
                }
            }
            hSubItem = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
        }
        else if(strcmp(pprotoChild->node_name,"t") ==0)
        {
            strTempName<<"发送时间(t): "+QString(pprotoChild->node_desc);
            hSubItem = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
        }
        else if(strcmp(pprotoChild->node_name,"stNum") ==0)
        {
            strTempName<<"状态计数(stNum): "+QString(pprotoChild->node_desc);
            hSubItem = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
        }
        else if(strcmp(pprotoChild->node_name,"sqNum") ==0)
        {
            strTempName<<"序号(sqNum): "+QString(pprotoChild->node_desc);
            hSubItem = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
        }
        else if(strcmp(pprotoChild->node_name,"test") ==0)
        {
            strTempName<<"检修标志(test): "+QString(pprotoChild->node_desc);
            if(strcmp(pprotoChild->node_desc,"True") == 0)
            {
                pprotoChild->errortype = 2;
            }
            hSubItem = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
        }
        else if(strcmp(pprotoChild->node_name,"confRev") ==0)
        {
            strTempName<<"配置版本(confRev): "+QString(pprotoChild->node_desc);
            if(pIed != NULL)
            {
                if(atoi(pprotoChild->node_desc) != pIed->nconfrev)
                {
                    strTempName<<"配置版本(confRev): "+QString(pprotoChild->node_desc)+" 配置为:"+QString::number(pIed->nconfrev);
                    pprotoChild->errortype = 2;
                }
            }
            hSubItem = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
        }
        else if(strcmp(pprotoChild->node_name,"ndsCom") ==0)
        {
            strTempName<<"需要重新配置(ndsCom): "+QString(pprotoChild->node_desc);
            hSubItem = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
        }
        else if(strcmp(pprotoChild->node_name,"numDatSetEntries") ==0)
        {
            if(nconfigchannelsize != -1 && nconfigchannelsize != atoi(pprotoChild->node_desc))
            {
                strTempName<<"数据集条目数(numDatSetEntries): "+QString(pprotoChild->node_desc)+" 数据集配置条目数："+QString(pprotoChild->node_desc)+QString::number(nconfigchannelsize);
                pprotoChild->errortype = 2;//数据集条目不一致
            }
            else
            {
                strTempName<<"数据集条目数(numDatSetEntries): "+QString(pprotoChild->node_desc);
            }
            hSubItem = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
        }
        else if(strcmp(pprotoChild->node_name,"allData") ==0)//所有数据
        {
            if(nconfigchannelsize != -1 && nconfigchannelsize !=pprotoChild->childlist_size)
            {
                strTempName<<"详细数据: "+QString::number(pprotoChild->childlist_size)+"项 配置为："+QString::number(nconfigchannelsize)+"项";
                pprotoChild->errortype = 2;
            }
            else
            {
                strTempName<<"详细数据: "+QString::number(pprotoChild->childlist_size);
            }
            hItemAsduData = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hItemAsduData->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hItemAsduData;//记忆本节点的位置
            //开始填充详细数据
            int ndatasize = pprotoChild->childlist_size;
            if(ndatasize > pGooseInfoStruct->n_numDataStructs)
            {
                ndatasize = pGooseInfoStruct->n_numDataStructs;
            }
            pprotoChildChild = pprotoChild->pchildlist;
            for(int k = 0 ; k < ndatasize; k++)
            {
                QString change;
                strTempName.clear();
                change.sprintf("数据%02d: %s (%s)",k+1,pGooseInfoStruct->p_data_struct[k].c_data,
                                    keywords61850Ts->GetMmsDataTypeDesc(pGooseInfoStruct->p_data_struct[k].n_data_type).c_str());
                strTempName<<change;
                if(pRelation != NULL)
                {
                    if(k < pRelation->n_yx_channel_num)
                    {
                        if(pGooseInfoStruct->p_data_struct[k].n_data_type != pRelation->pyx_channel_list[k]->n61850_data_type)
                        {
                            change.sprintf("数据%02d %s: %s (%s,数据类型不匹配,配置为:%s)",k+1,pRelation->pyx_channel_list[k]->cdesc,
                                pGooseInfoStruct->p_data_struct[k].c_data,
                                keywords61850Ts->GetMmsDataTypeDesc(pGooseInfoStruct->p_data_struct[k].n_data_type).c_str(),
                                keywords61850Ts->GetMmsDataTypeDesc(pRelation->pyx_channel_list[k]->n61850_data_type).c_str());
                            strTempName << change;
                            pprotoChildChild->errortype = 2;//标记异常
                        }
                        else
                        {
                            change.sprintf("数据%02d %s: %s (%s)",k+1,pRelation->pyx_channel_list[k]->cdesc,
                                                            pGooseInfoStruct->p_data_struct[k].c_data,
                                                            keywords61850Ts->GetMmsDataTypeDesc(pGooseInfoStruct->p_data_struct[k].n_data_type).c_str());
                            strTempName<<change;
                        }
                    }
                }
                //如果有GOOSE变位，需挑出GOOSE变位信息
                if(pMsgGinInfo->beventanalyzedgoose && pMsgGinInfo->pPreCapMsg != NULL)
                {
                    GOOSE_INFO_STRUCT* pGooseInfoStructPre = (GOOSE_INFO_STRUCT*) pMsgGinInfo->pPreCapMsg->pparserdstruct;
                    if(JudgeGooseStateChange(k,pMsgGinInfo))
                    {
                        pprotoChildChild->errortype = 1; //发生事件变位
                    }
                }
                hSubItem = new QTreeWidgetItem(hItemAsduData,strTempName);
                hSubItem->setData(1,Qt::UserRole,QVariant::fromValue(pprotoChildChild));
                pprotoChildChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
                if(pGooseInfoStruct->p_data_struct[k].n_data_type == 2)//结构
                {
                    int nsubsubsize = pprotoChildChild->childlist_size;
                    if(nsubsubsize > pGooseInfoStruct->p_data_struct[k].n_struct_num)
                    {
                        nsubsubsize = pGooseInfoStruct->p_data_struct[k].n_struct_num;
                    }
                    for(int kk = 0; kk < nsubsubsize; kk++)//结构
                    {
                        strTempName.clear();
                        change.sprintf("数据%02d: %s (%s)",kk+1,pGooseInfoStruct->p_data_struct[k].p_struct[kk].c_data,
                                                    QString::fromLocal8Bit(keywords61850Ts->GetMmsDataTypeDesc(pGooseInfoStruct->p_data_struct[k].p_struct[kk].n_data_type).c_str()).toStdString().c_str());
                        strTempName<<change;
                        hsubsubItem = new QTreeWidgetItem(hSubItem,strTempName);//加入树节点
                        hsubsubItem->setData(1,Qt::UserRole,QVariant::fromValue(&pprotoChildChild->pchildlist[kk]));
                        pprotoChildChild->pchildlist[kk].pkt_usrview = (void*) hsubsubItem;//记忆本节点的位置
                        //如果有GOOSE变位，需挑出GOOSE变位信息
                        if(pMsgGinInfo->beventanalyzedgoose && pMsgGinInfo->pPreCapMsg != NULL)
                        {
                            GOOSE_INFO_STRUCT* pGooseInfoStructPre = (GOOSE_INFO_STRUCT*) pMsgGinInfo->pPreCapMsg->pparserdstruct;
                            if(strcmp(pGooseInfoStruct->p_data_struct[k].p_struct[kk].c_data,pGooseInfoStructPre->p_data_struct[k].p_struct[kk].c_data)!= 0
                                && pGooseInfoStruct->p_data_struct[k].p_struct[kk].n_data_type != 17)
                            {
                                pprotoChildChild->pchildlist[kk].errortype = 1; //发生事件变位
                            }
                        }//与上一帧有变化的处理
                    }//只填写一层结构
                    //Expand(hSubItem,TVE_EXPAND);
                    ui->messageTreeWidget->expandItem(hSubItem);
                }//填充结构
                pprotoChildChild++;
            }
        }
        else
        {
            strTempName<<QString(pprotoChild->node_name_detail)+": "+QString(pprotoChild->node_desc);
            hSubItem = new QTreeWidgetItem(hItemSeqAsdu,strTempName);
            hSubItem->setData(0,Qt::UserRole,QVariant::fromValue(pprotoChild));
            pprotoChild->pkt_usrview = (void*) hSubItem;//记忆本节点的位置
        }
        pprotoChild++;
    }
    ui->messageTreeWidget->expandItem(hItemSeqAsdu);
    ui->messageTreeWidget->expandItem(hItem);
    //Expand(hItemSeqAsdu,TVE_EXPAND);
    //Expand(hItem,TVE_EXPAND);
}
/**
 * @brief detailedMessageView::JudgeGooseStateChange 变位信息判断
 * @param nChannelNum 通道号
 * @param pMsgGinInfo 报文帧信息结构体
 * @return
 */
BOOL detailedMessageView::JudgeGooseStateChange(int nChannelNum, CAPMSGGININFO *pMsgGinInfo)
{
    MESSAGE_ERROR_INFO_ARRAY_STRUCT* pArrayStruct = (MESSAGE_ERROR_INFO_ARRAY_STRUCT*) pMsgGinInfo->pap_analyzed_info;
    if(pArrayStruct == NULL)
        return FALSE;
    for(int i = 0; i< pArrayStruct->n_msg_err; i++)
    {
        for(int j = 0; j < pArrayStruct->p_msg_err[i].n_num_asduerr; j++)//该数据项是否错误
        {
            if(pArrayStruct->p_msg_err[i].p_asduerr[j].n_num_errcode <= 0)
                continue;
            if(pArrayStruct->p_msg_err[i].p_asduerr[j].p_errcode[0] == 24 && pArrayStruct->p_msg_err[i].p_asduerr[j].n_asdu_no == nChannelNum)//状态变位
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}
/**
 * @brief detailedMessageView::FillParseredMsg_MmsStruct_Detail 详细填充mms结构
 * @param pMmsInfoStruct MMS报文结构
 * @param hparentItem 树节点
 */
void detailedMessageView::FillParseredMsg_MmsStruct_Detail(MMS_INFO_STRUCT*  pMmsInfoStruct,QTreeWidgetItem *hparentItem)
{
    QString strTempName;
    QTreeWidgetItem *hItem,*childItem;
    //输出参引列表
    if(pMmsInfoStruct->n_variable_num > 0)
    {
        strTempName.sprintf("参引条目: [%d项]",pMmsInfoStruct->n_variable_num);
        hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        for(int i = 0; i < pMmsInfoStruct->n_variable_num; i++)
        {
            strTempName.sprintf("%02d_参引[domainid$itemid]: %s%s",i+1,pMmsInfoStruct->p_variable_struct[i].c_domain_id,pMmsInfoStruct->p_variable_struct[i].c_item_id);
            childItem = new QTreeWidgetItem(hItem,QStringList(strTempName));
        }
        ui->mmsMessageTreeWidget->expandItem(hItem);
    }
    //输出数据列表
    if(pMmsInfoStruct->n_data_num > 0)
    {
        strTempName.sprintf("详细数据: [%d项]",pMmsInfoStruct->n_data_num);
        hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        for(int i = 0; i < pMmsInfoStruct->n_data_num; i++)
        {
            FillParseredMsg_MmsStruct_Detail_MMS_DATA(pMmsInfoStruct->p_data_struct[i],hItem,i+1);
        }
        ui->mmsMessageTreeWidget->expandItem(hItem);
    }
}
/**
 * @brief detailedMessageView::FillParseredMsg_MmsStruct_Detail_MMS_DATA 详细填充mms结构中的数据
 * @param mms_data_struct MMS报文数据结构
 * @param hparentItem 树节点
 * @param nseqInlist 位置
 */
void detailedMessageView::FillParseredMsg_MmsStruct_Detail_MMS_DATA(MMS_DATA_STRUCT mms_data_struct,QTreeWidgetItem *hparentItem,int nseqInlist)
{
    QString strTempName;
    QTreeWidgetItem *hItem;
    if(mms_data_struct.n_data_type == 2)//n_mdt_structure
    {
        strTempName.sprintf("%02d_结构[%s]",nseqInlist,keywords61850Ts->GetMmsDataTypeDesc(mms_data_struct.n_data_type).c_str());
        hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        ui->mmsMessageTreeWidget->expandItem(hItem);
    }
    else
    {
        strTempName.sprintf("%02d_值[%s]:%s",nseqInlist,keywords61850Ts->GetMmsDataTypeDesc(mms_data_struct.n_data_type).c_str(),QString::fromLocal8Bit(mms_data_struct.c_data).toStdString().c_str());
        hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        ui->mmsMessageTreeWidget->expandItem(hItem);
    }
    for(int i = 0;i < mms_data_struct.n_struct_num;i++)
    {
        FillParseredMsg_MmsStruct_Detail_MMS_DATA(mms_data_struct.p_struct[i],hItem,i+1);
        ui->mmsMessageTreeWidget->expandItem(hItem);
    }
}
/**
 * @brief detailedMessageView::FillParseredMsg_MmsStruct_FILEDIRECTORY 填充MMS文件目录服务结构
 * @param pMmsInfoStruct MMS报文信息结构
 * @param hparentItem 树节点
 */
void detailedMessageView::FillParseredMsg_MmsStruct_FILEDIRECTORY(MMS_INFO_STRUCT*  pMmsInfoStruct,QTreeWidgetItem *hparentItem)
{
    QString strTempName;
    QTreeWidgetItem *hItem,*hsubItem,*childItemForP,*childItemForS;
    if(strlen(pMmsInfoStruct->mms_file_directory.c_file_request_directory))//目录有内容
    {
        strTempName.sprintf("请求的文件目录名称：[%s]",pMmsInfoStruct->mms_file_directory.c_file_request_directory);
        childItemForP = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
    }
    else
    {
        strTempName.sprintf("文件列表：[%d项]",pMmsInfoStruct->mms_file_directory.n_file_num);
        hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));

        if( pMmsInfoStruct->mms_file_directory.n_file_num > 0)
        {
            for(int i = 0; i < pMmsInfoStruct->mms_file_directory.n_file_num; i++)
            {
                strTempName.sprintf("文件%02d: %s",i+1,pMmsInfoStruct->mms_file_directory.p_file_entry_struct[i].c_file_name);
                hsubItem = new QTreeWidgetItem(hItem,QStringList(strTempName));

                strTempName.sprintf("大小(字节)：[%d]",pMmsInfoStruct->mms_file_directory.p_file_entry_struct[i].mms_file_attr.n_size_of_file);
                childItemForS = new QTreeWidgetItem(hsubItem,QStringList(strTempName));

                strTempName.sprintf("最后修改时间: %s",pMmsInfoStruct->mms_file_directory.p_file_entry_struct[i].mms_file_attr.c_time_last_modified);
                childItemForS = new QTreeWidgetItem(hsubItem,QStringList(strTempName));
                ui->mmsMessageTreeWidget->expandItem(hsubItem);
            }
            ui->mmsMessageTreeWidget->expandItem(hItem);
        }
    }
}
/**
 * @brief detailedMessageView::FillParseredMsg_MmsStruct_Init 填充MMS服务-初始化服务结构
 * @param pMmsInfoStruct MMS报文信息结构
 * @param hparentItem 树节点
 */
void detailedMessageView::FillParseredMsg_MmsStruct_Init(MMS_INFO_STRUCT*  pMmsInfoStruct,QTreeWidgetItem *hparentItem)
{
    QString strTempName;
    QTreeWidgetItem *hItem;
    strTempName.sprintf("MMS_PDU最大长度: [%d]",pMmsInfoStruct->struct_comm_manage_param.n_localDetailCall);
    hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));

    strTempName.sprintf("最大数据结构迁套层次: [%d]",pMmsInfoStruct->struct_comm_manage_param.n_dataStructNestingLevel);
    hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));

    strTempName.sprintf("建议最大主叫服务器数目: [%d]",pMmsInfoStruct->struct_comm_manage_param.n_maxOutstandingCalling);
    hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));

    strTempName.sprintf("建议最大被叫服务器数目: [%d]",pMmsInfoStruct->struct_comm_manage_param.n_maxOutstandingCalled);
    hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));

    strTempName.sprintf("版本号:[%d]",pMmsInfoStruct->struct_comm_manage_param.struct_init_Detail.n_VersionNum);
    hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));

    strTempName.sprintf("建议参数: %s",pMmsInfoStruct->struct_comm_manage_param.struct_init_Detail.c_paramCBB);
    hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));

    strTempName.sprintf("主叫或被叫支持的服务: %s",pMmsInfoStruct->struct_comm_manage_param.struct_init_Detail.c_supportedCall);
    hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
}
/*填充MMS服务-报告服务*/
//显示详细报告
//---------------------------------报告结构----------------------------------//
//1-报告标识 RPTID
//2-OptFlds:包含在报告中的选项域 共8位 从左到右
// --2.0 保留未用
// --2.1 顺序序号     SeqNum
// --2.2 报告生成时间 Report Time Stamp
// --2.3 报告原因  Reason for Inclusion (dchag数据变化,qchag品质变化,dupd数据刷新,ingegrity完整性，general interrogation 总召唤)
// --2.4 数据集名称 DataSet Name
// --2.5 数据参引 Data Reference
// --2.6 缓冲溢出 Buffer Overflow
// --2.7 条目号   EntryID
// --2.8 版本号   Conf-Rev
// --2.9 后随多个报告段 Segmentation
//3.0-顺序号      SqNum                       ----OptFlds2.1 中顺序序号置TRUE
//3.1-子顺序号    SubSqNum                    ----OptFlds2.1 中顺序序号置TRUE
//3.2-同样顺序号有多个 MoreSegmentFollow      ----OptFlds2.1 中顺序序号置TRUE
//4-报告条目时间 TimeOfEntry                ----OptFlds2.2  报告生成时间置TRUE
//5-数据集名称  DataSet                     ----OptFlds2.4 数据集名称置TRUE
//6-缓冲溢出                                ----OptFlds2.6  缓冲溢出置TRUE
//7-报告编号 EntryID                        ----OptFlds2.7  条目号置TRUE
//8-版本号                                  ----OptFlds2.8  版本号置TRUE
//9-EntraData(1-n)
//10.1 数据参引(1-n)  DataRef 各自的数据属性参引    ----OptFlds2.5  数据参引置TRUE
//10.2 值      （1-n）Value  数据值
//10.3 报告原因(1-n)  TriggerConditions           ----OptFlds2.3 报告原因置TRUE 是个6位的bit_string
//从左到右 1.备用 2.data change 3.quality change  4.data update 5.integrity 6.gi（总召）
void detailedMessageView::FillParseredMsg_MmsStruct_Report(MMS_INFO_STRUCT*  pMmsInfoStruct,QTreeWidgetItem *hparentItem)
{
    QString strTempName,strTemp;
    char c_item_desc[256];
    string strdesc;
    QTreeWidgetItem *hItem,*hsubItem,*childItemForP;
    SCD_ITEM_LIST* pdsItemList = NULL;
    SCD_ITEM     * pdoiItem  = NULL;
    //输出数据列表
    if(pMmsInfoStruct->n_data_num < 2)
        return;
    CTsScdParserWrapper *tsScdParser = CTsScdParserWrapper::getInstance();
    //ChineseCode myConvert;
    //报告标识
    strTempName.sprintf("报告标识：%s",pMmsInfoStruct->p_data_struct[0].c_data);
    childItemForP = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
    //报告选项
    int n_optlist[9];        //在抬头中的选项
    int optlist_entrydata[2];//在数据中的选项
    memset(n_optlist,0,sizeof(n_optlist));
    memset(optlist_entrydata,0,sizeof(optlist_entrydata));
    FillParseredMsg_MmsStruct_Report_OptFlds(pMmsInfoStruct->p_data_struct[1],hparentItem,n_optlist,optlist_entrydata);
    int nfieds_before_data = 2;
    bool bMoreSegmentsFllows = false;
    for(int i = 0;i < 9;i++)
    {
        if(0 == n_optlist[i])
            break;
        strdesc.clear();
        memset(c_item_desc,0,256);
        if(4 == n_optlist[i])//包含数据集队列
        {
            tsScdParser->GetItemDesc_ByRef_I(pMmsInfoStruct->p_data_struct[2+i].c_data,c_item_desc,TS_SCD_ITEM_TYPE_SCDDataSet);
            if(strlen(c_item_desc))
            {
                //myConvert.UTF_8ToGB2312(strdesc,c_item_desc,strlen(c_item_desc));
                strdesc = c_item_desc;
            }
        }
        if( 9 == n_optlist[i])
        {
            bMoreSegmentsFllows = true;
        }
        if(strlen(strdesc.c_str()) > 0)//增加描述
        {
            strTempName.sprintf("%s: %s",GetOptNameByIndex(n_optlist[i]).toStdString().c_str(),QString::fromLocal8Bit(strdesc.c_str()).toStdString().c_str());
        }
        else
        {
            strTempName.sprintf("%s: %s",GetOptNameByIndex(n_optlist[i]).toStdString().c_str(),pMmsInfoStruct->p_data_struct[2+i].c_data);
        }
        childItemForP = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        nfieds_before_data++;//有效个数计数
    }
    if(bMoreSegmentsFllows)//同样序号有多个报告段
    {
        strTempName.sprintf("同样序号有多个报告段[MoreSegmentsFllow]: %s",pMmsInfoStruct->p_data_struct[nfieds_before_data].c_data);
        childItemForP = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        nfieds_before_data ++;
    }
    nfieds_before_data ++;//需要减去数据的列表
    int ndatafields = pMmsInfoStruct->n_data_num - nfieds_before_data;//实际的个数
    if(optlist_entrydata[0] > 0 && optlist_entrydata[1] >0)//参引、触发选项
    {
        if(ndatafields %3 > 0)//格式异常
        {
            strTempName.sprintf("事件内容与OptFlds标记内容不一致",ndatafields);
            childItemForP = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        }
        nfieds_before_data += ndatafields %3;//跳过不一致
        ndatafields = ndatafields/3;
        strTempName.sprintf("详细数据:%d项",ndatafields);//pMmsInfoStruct->p_data_struct[nfieds_before_data-1].c_data);//事件在数据集中的序列
        hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        for(int i = 0;i < ndatafields;i++)
        {
            strdesc.clear();
            pdoiItem = tsScdParser->Get_DoiItem_BytFcdaRef_I(pMmsInfoStruct->p_data_struct[i+nfieds_before_data].c_data);
            if(pdoiItem != NULL)
            {
                strdesc = pdoiItem->c_item_desc;
            }
            if(strlen(strdesc.c_str()) > 0)
            {
                strTempName.sprintf("数据_%d:%s",i+1,QString::fromLocal8Bit(strdesc.c_str()).toStdString().c_str());//参引
            }
            else
            {
                strTempName.sprintf("数据_%d: %s",i+1,pMmsInfoStruct->p_data_struct[i+nfieds_before_data].c_data);//参引
            }
            tsScdParser->Free_Scd_Item_I(pdoiItem);
            hsubItem  = new QTreeWidgetItem(hItem,QStringList(strTempName));
            FillParseredMsg_MmsStruct_Detail_MMS_DATA(pMmsInfoStruct->p_data_struct[i+nfieds_before_data+ndatafields],hsubItem,1);//值
            FillParseredMsg_MmsStruct_Report_TriggerConditions(pMmsInfoStruct->p_data_struct[nfieds_before_data+i+ndatafields+ndatafields],hsubItem);//更新原因
            ui->mmsMessageTreeWidget->expandItem(hsubItem);
        }
        ui->mmsMessageTreeWidget->expandItem(hItem);
    }
    else if(optlist_entrydata[0] == 0 && optlist_entrydata[1] > 0)//仅参引
    {
        if(ndatafields %2 > 0)//格式异常
        {
            strTempName.sprintf("事件内容与OptFlds标记内容不一致",ndatafields);
            childItemForP = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        }
        nfieds_before_data += ndatafields %2;//跳过不一致
        ndatafields = ndatafields/2;
        strTempName.sprintf("详细数据:%d项",ndatafields);//,pMmsInfoStruct->p_data_struct[nfieds_before_data-1].c_data);//事件在数据集中的序列
        hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        for(int i = 0;i < ndatafields;i++)
        {
            strdesc.clear();
            pdoiItem = tsScdParser->Get_DoiItem_BytFcdaRef_I(pMmsInfoStruct->p_data_struct[i+nfieds_before_data].c_data);
            if(pdoiItem != NULL)
            {
                strdesc = pdoiItem->c_item_desc;
            }
            if(strlen(strdesc.c_str()) > 0)
            {
                strTempName.sprintf("%s_[%s]",QString::fromLocal8Bit(strdesc.c_str()).toStdString().c_str(),pMmsInfoStruct->p_data_struct[i+nfieds_before_data].c_data);//参引
            }
            else
            {
                strTempName.sprintf("数据_%d: %s",i+1,pMmsInfoStruct->p_data_struct[i+ nfieds_before_data].c_data);//参引
            }
            tsScdParser->Free_Scd_Item_I(pdoiItem);

            hsubItem  = new QTreeWidgetItem(hItem,QStringList(strTempName));
            FillParseredMsg_MmsStruct_Detail_MMS_DATA(pMmsInfoStruct->p_data_struct[i+nfieds_before_data+ndatafields],hsubItem,1);//值
            ui->mmsMessageTreeWidget->expandItem(hsubItem);
        }
        ui->mmsMessageTreeWidget->expandItem(hItem);
    }
    else if(optlist_entrydata[0] > 0 && optlist_entrydata[1] == 0)//仅触发选项
    {
        if(ndatafields %2 > 0)//格式异常
        {
            strTempName.sprintf("事件内容与OptFlds标记内容不一致",ndatafields);
            childItemForP = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        }
        nfieds_before_data += ndatafields %2;//跳过不一致
        ndatafields = ndatafields/2;
        strTempName.sprintf("详细数据:%d项",ndatafields);//,pMmsInfoStruct->p_data_struct[nfieds_before_data-1].c_data);//事件在数据集中的序列
        hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        for(int i = 0;i < ndatafields;i++)
        {
            strTempName.sprintf("数据_%d",i+1);//参引
            hsubItem  = new QTreeWidgetItem(hItem,QStringList(strTempName));
            FillParseredMsg_MmsStruct_Detail_MMS_DATA(pMmsInfoStruct->p_data_struct[i+nfieds_before_data],hsubItem,1);//值
            FillParseredMsg_MmsStruct_Report_TriggerConditions(pMmsInfoStruct->p_data_struct[i+nfieds_before_data+ndatafields],hsubItem);//更新原因
            ui->mmsMessageTreeWidget->expandItem(hsubItem);
        }
        ui->mmsMessageTreeWidget->expandItem(hItem);
    }
    else//仅数据
    {
        strTempName.sprintf("事件个数:[%d]",ndatafields);//,pMmsInfoStruct->p_data_struct[nfieds_before_data-1].c_data);//事件在数据集中的序列
        hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        for(int i = 0;i < ndatafields;i++)
        {
            FillParseredMsg_MmsStruct_Detail_MMS_DATA(pMmsInfoStruct->p_data_struct[i+2+nfieds_before_data+ndatafields],hItem,i);//值
        }
        ui->mmsMessageTreeWidget->expandItem(hItem);
    }
    if(pdsItemList)//释放申请的资源
        tsScdParser->Free_Scd_ItemList_I(pdsItemList);
}
//2-OptFlds:包含在报告中的选项域 共10位 从左到右
// --2.0 保留未用
// --2.1 顺序序号     SeqNum
// --2.2 报告生成时间 Report Time Stamp
// --2.3 报告原因  Reason for Inclusion (dchag数据变化,qchag品质变化,dupd数据刷新,ingegrity完整性，general interrogation 总召唤)
// --2.4 数据集名称 DataSet Name
// --2.5 数据参引 Data Reference
// --2.6 缓冲溢出 Buffer Overflow
// --2.7 条目号   EntryID
// --2.8 版本号   Conf-Rev
// --2.9 后随多个报告段 Segmentation
void detailedMessageView::FillParseredMsg_MmsStruct_Report_OptFlds(MMS_DATA_STRUCT data_struct,QTreeWidgetItem *hparentItem,int *n_optlist,int *optlist_entrydata)
{
    int nstrlen = strlen(data_struct.c_data);
    QString strTempName,strTemp;
    QTreeWidgetItem *hItem,*childItemForP,*childItemForH;
    if(nstrlen != 10)//格式判断
    {
        strTempName.sprintf("OptFlds选项域异常，应为10项，实际%d项",nstrlen);
        childItemForP = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        return;
    }
    int ntemp = 0;
    strTempName.sprintf("OptFlds选项域: %s",data_struct.c_data);
    hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
    strTempName.sprintf("%c......... : 保留未用",data_struct.c_data[0]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    strTempName.sprintf(".%c........ : 顺序序号[SeqNum]",data_struct.c_data[1]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[1] == '1')
    {
        *n_optlist = 1;
        n_optlist ++;
        strTemp += "顺序序号|";
    }
    strTempName.sprintf("..%c....... : 报告生成时间[Report Time Stamp]",data_struct.c_data[2]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[2] == '1')
    {
        *n_optlist = 2;
        n_optlist ++;
        strTemp += "报告生成时间|";
    }
    strTempName.sprintf("...%c...... : 报告原因[Reason for Inclusion]",data_struct.c_data[3]);//此选项为数据中的选项
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[3] == '1')
    {
//		*n_optlist = 3;
//		n_optlist ++;
        *optlist_entrydata = 3;
        optlist_entrydata ++;
        strTemp += "报告原因|";
    }
    strTempName.sprintf("....%c..... : 数据集名称[DataSet Name]",data_struct.c_data[4]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[4] == '1')
    {
        *n_optlist = 4;
        n_optlist ++;
        strTemp += "数据集名称|";
    }
    strTempName.sprintf(".....%c.... : 数据参引[Data Reference]",data_struct.c_data[5]);//此选项为数据中的选项
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[5] == '1')
    {
//		*n_optlist = 5;
//		n_optlist ++;
        *optlist_entrydata = 5;
        optlist_entrydata ++;
        strTemp += "数据参引|";
    }
    strTempName.sprintf("......%c... : 缓冲溢出[Buffer Overflow]",data_struct.c_data[6]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[6] == '1')
    {
        *n_optlist = 6;
        n_optlist ++;
        strTemp += "缓冲溢出|";
    }
    strTempName.sprintf(".......%c.. : 条目号[EntryID]",data_struct.c_data[7]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[7] == '1')
    {
        *n_optlist = 7;
        n_optlist ++;
        strTemp += "条目号|";
    }
    strTempName.sprintf("........%c. : 版本号[Conf-Rev]",data_struct.c_data[8]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[8] == '1')
    {
        *n_optlist = 8;
        n_optlist ++;
        strTemp += "版本号|";
    }
    strTempName.sprintf(".........%c : 后随多个报告段[Segmentation]",data_struct.c_data[9]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[9] == '1')
    {
        *n_optlist = 9;
        n_optlist ++;
        strTemp += "后随多个报告段|";
    }
    if(strTemp.length())
        strTemp = strTemp.left(strTemp.length()-1);
    strTempName.sprintf("OptFlds选项域: %s[%s]",strTemp.toStdString().c_str(),data_struct.c_data);
    hItem->setData(0,Qt::DisplayRole,QVariant::fromValue(strTempName));
    //SetItemText(hItem,strTempName);
}
//报告原因[Reason for Inclusion]
//10.3 报告原因(1-n)  TriggerConditions           ----OptFlds2.3 报告原因置TRUE 是个6位的bit_string
//从左到右 1.备用 2.data change 3.quality change  4.data update 5.integrity 6.gi（总召）
void detailedMessageView::FillParseredMsg_MmsStruct_Report_TriggerConditions(MMS_DATA_STRUCT data_struct,QTreeWidgetItem *hparentItem)
{
    int nstrlen = strlen(data_struct.c_data);
    QString strTempName;
    QTreeWidgetItem *hItem,*childItemForP,*childItemForH;
    if(nstrlen != 6)//格式判断
    {
        strTempName.sprintf("报告原因[Reason for Inclusion]内容异常，应为6项，实际%d项",nstrlen);
        childItemForP = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
        return;
    }
    strTempName.sprintf("报告原因: %s",data_struct.c_data);
    hItem = new QTreeWidgetItem(hparentItem,QStringList(strTempName));
    QString strTemp;
//	strTempName.Format("%c.....: 备用",data_struct.c_data[0]);
//	InsertItem(strTempName,1,9,hItem);
    strTempName.sprintf(".%c....: 值变化[data change]",data_struct.c_data[1]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[1]=='1')
    {
        strTemp += "值变化|";
    }
    strTempName.sprintf("..%c...: 品质变化[quality change]",data_struct.c_data[2]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[2]=='1')
    {
        strTemp += "品质变化|";
    }
    strTempName.sprintf("...%c..: 数据更新[data update]",data_struct.c_data[3]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[3]=='1')
    {
        strTemp += "数据更新|";
    }
    strTempName.sprintf("....%c.: 数据完整性[integrity]",data_struct.c_data[4]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[4]=='1')
    {
        strTemp += "数据完整性|";
    }
    strTempName.sprintf(".....%c: 总召[pi]",data_struct.c_data[5]);
    childItemForH = new QTreeWidgetItem(hItem,QStringList(strTempName));
    if(data_struct.c_data[5]=='1')
    {
        strTemp += "总召|";
    }
    if(strTemp.length())
        strTemp = strTemp.left(strTemp.length()-1);
    strTempName.sprintf("报告原因: %s[%s]",strTemp.toStdString().c_str(),data_struct.c_data);
    hItem->setData(0,Qt::DisplayRole,QVariant::fromValue(strTempName));
    //SetItemText(hItem,strTempName);
}
/**
 * @brief detailedMessageView::GetOptNameByIndex 获取节点名称
 * @param nIndex
 * @return
 */
QString detailedMessageView::GetOptNameByIndex(int nIndex)
{
    QString strOptDesc;
    switch(nIndex)
    {
        case 1:
            strOptDesc = "顺序序号[SeqNum]";
            break;
        case 2:
            strOptDesc = "报告生成时间[Report Time Stamp]";
            break;
        case 4:
            strOptDesc = "数据集名称[DataSet Name]";
            break;
        case 6:
            strOptDesc = "缓冲溢出[Buffer Overflow]";
            break;
        case 7:
            strOptDesc = "条目号[EntryID]";
            break;
        case 8:
            strOptDesc = "版本号[Conf-Rev]";
            break;
        case 9:
            strOptDesc = "子顺序号[SubSqNum]";
            break;
        default:
            break;
    }
    return strOptDesc;
}
