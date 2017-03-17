#ifndef DETAILEDMESSAGEVIEW_H
#define DETAILEDMESSAGEVIEW_H

#include <QWidget>
#include "ui_detailedmessageview.h"
#include "capanalyzerstruct.h"
#include "xjlib.h"
#include "qhexedit.h"
namespace Ui {
class detailedMessageView;
}


class QQuickWidget;
class detailedMessageView : public QWidget
{
    Q_OBJECT

public:
    explicit detailedMessageView(QWidget *parent = 0);
    ~detailedMessageView();
    void receiveMsgDetail(CAPMSGGININFO *pMsgGinInfo);
    void HexContrl2TreeControl(int nPosition);
    XJ_PROTO_NODE * GetProtoNodeByHexPosition(int nPosition,XJ_PROTO_NODE* pParentNode);

private:
    void setupMoreUi();

    QQuickWidget *m_qwTimeDisper;
    QQuickWidget *m_qwHarmonic;
    QQuickWidget *m_qwVector;
    QQuickWidget *m_qwSequence;

private:
    Ui::detailedMessageView *ui;
    void WsWriteMsgDetail(CAPMSGGININFO* pMsgGinInfo);//显示详细信息
    void FillParseredMsg_PacketDesc(CAPMSGGININFO* pMsgGinInfo);//填充详细报文信息头
    void FillParseredMsg_All(CAPMSGGININFO* pMsgGinInfo);/*填充其它消息类型*/
    void TreeCtrlOutputXjPrototree(XJ_PROTO_NODE * pprototreelist,QTreeWidgetItem *itemSuper,BOOL bTopLevel,BOOL bExpand=FALSE);
    /*填充SMV结构*/
    void FillParseredMsg_SMV92Struct(CAPMSGGININFO* pMsgGinInfo);
    /*填充goose结构*/
    void FillParseredMsg_GooseStruct(CAPMSGGININFO* pMsgGinInfo);
    BOOL JudgeGooseStateChange(int nChannelNum,CAPMSGGININFO* pMsgGinInfo);
    /*填充mms结构*/
    void FillParseredMsg_MmsStruct(CAPMSGGININFO* pMsgGinInfo,BOOL bInsertConnect);
    void FillMsgDetail(CAPMSGGININFO* pMsgGinInfo);//填充MMS控制
    /*详细填充mms结构*/
    void FillParseredMsg_MmsStruct_Detail(MMS_INFO_STRUCT*  pMmsInfoStruct,QTreeWidgetItem *hparentItem);
    /*填充MMS文件目录服务结构*/
    void FillParseredMsg_MmsStruct_FILEDIRECTORY(MMS_INFO_STRUCT*  pMmsInfoStruct,QTreeWidgetItem *hparentItem);
    /*填充MMS服务-初始化服务结构*/
    void FillParseredMsg_MmsStruct_Init(MMS_INFO_STRUCT*  pMmsInfoStruct,QTreeWidgetItem *hparentItem);
    /*填充MMS服务-报告服务*/
    void FillParseredMsg_MmsStruct_Report(MMS_INFO_STRUCT*  pMmsInfoStruct,QTreeWidgetItem *hparentItem);
    void FillParseredMsg_MmsStruct_Report_OptFlds(MMS_DATA_STRUCT data_struct,QTreeWidgetItem *hparentItem,int *n_optlist,int *optlist_entrydata);
    void FillParseredMsg_MmsStruct_Report_TriggerConditions(MMS_DATA_STRUCT data_struct,QTreeWidgetItem *hparentItem);
    QString GetOptNameByIndex(int nIndex);
    /*详细填充mms结构中的数据*/
    void FillParseredMsg_MmsStruct_Detail_MMS_DATA(MMS_DATA_STRUCT mms_data_struct,QTreeWidgetItem *hparentItem,int nseqInlist);
    QString GetLinkDesc(CAPMSGGININFO* pMsgGinInfo);


private:
    QTreeWidgetItem *itemDetailedRoot;//详细报文解析根节点
    QWidget *mmsTab;//mms标签页
    QHexEdit *hexEdit;//16进制
    CAPMSGGININFO* m_pMsgGinInfo;
private slots:
    void OnMySelectTreeItem(QTreeWidgetItem * item, int column);
};
Q_DECLARE_METATYPE( XJ_PROTO_NODE*)
#endif // DETAILEDMESSAGEVIEW_H
