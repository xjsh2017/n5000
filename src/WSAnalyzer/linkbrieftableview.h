#ifndef LINKBRIEFTABLEVIEW_H
#define LINKBRIEFTABLEVIEW_H
#include "cappackagesmnger.h"
#include "detailedmessageview.h"
#include <QWidget>

namespace Ui {
class linkBriefTableView;
}

class QQuickWidget;
class WaveAnalDataModel;
class linkBriefTableView : public QWidget
{
    Q_OBJECT

public:
    explicit linkBriefTableView(QWidget *parent = 0);
    ~linkBriefTableView();
    void receiveMsgDetail(CapPackagesMnger *m_cappackagesmnger,CAPCONNECTINFO *getSelectCapConnectInfo,int nDataShowtype,int linkType);
    void RefreshConnectDetail(CAPCONNECTINFO *pcapconnectinfo,int nDataShowtype,int npcapconnectType);
    void RefreshLinkBriefTab(CAPCONNECTINFO *pcapconnectinfo,int nDataShowtype);
    void FillTGridTreeControl(CAPCONNECTINFO *pcapconnectinfo, int nDataShowtype);
    void FillTGridTree(MAP_CAPMSGINFO m_pmsgginifo);
    void FillTGirdTreForTimeStyles();
    QString GetLinkDesc(CAPCONNECTINFO *pcapconnectinfo);
    void GetSMVLinkInfo(CAPCONNECTINFO *pcapconnectinfo, QString appid, QString mac1, QString mac2, QString ip1, QString ip2);
    /***获取各列分析结果字符串****/
    QString GetTimeTxt(CAPMSGGININFO* pCapMsgGinInfo,int nMode);
    QString GetListContent_Cloumn3(CAPMSGGININFO *pCapMsgGinInfo);
    QString GetListContent_Cloumn4(CAPMSGGININFO *pCapMsgGinInfo);
    QString GetListContent_Cloumn5(CAPMSGGININFO *pCapMsgGinInfo);
    QString GetListContent_Cloumn6(CAPMSGGININFO *pCapMsgGinInfo);
    QString GetListContent_Cloumn7(CAPMSGGININFO *pCapMsgGinInfo);
    QString GetSmvErrDesc(CAPMSGGININFO* pCapMsgGinInfo);
    QString GetGooseErrDesc(CAPMSGGININFO* pCapMsgGinInfo);

public slots:
    void UpdateWaveAnalDataModel(CapPackagesMnger *m_cappackagesmnger,CAPCONNECTINFO *getSelectCapConnectInfo,int nDataShowtype,int linkType);

private:
    Ui::linkBriefTableView *ui;
    detailedMessageView *messageView;

    QQuickWidget *m_qwWaveAnal; // 波形分析图
    WaveAnalDataModel *m_qwWaveData; // 波形分析数据

private:
    void fillLinkTree();

    MAP_CAPMSGINFO m_pmsgginifoSS;
    CapPackagesMnger ts_cappackagesmnger;
    CAPCONNECTINFO * m_pCapconnectinfoFillter;//连接过滤器    
    int  m_nFindErrPkPos;//记录告警帧位置
    int  m_nShowDataType;         //数据显示类型
    CAPMSGGININFO* GetSelectItemMsg(int nItemIndex);//获取选中信息包的信息
private slots:
    void OnMySelectBriefTableItem(int row, int column);//选中列表
    void OnToolbarNextErrPkt(bool isClicked);//下一帧报文
    void OnToolbarPreErrPkt(bool isClicked);//上一帧报文
    void OnToolbarExportAll();//导出全部报文
    void OnToolbarExportSel();//选中报文另存
    //CAPMSGGININFO* GetSelectItemMsg(int nItemIndex);//获取选中信息包的信息类型
    void OnToolbarPktFindLink();//定位链路
    void OnToolbarPktFilter();//报文过滤

    void currentChanged(int index); // tab页切换

signals:
    void wsWriteMsgDetail(CAPMSGGININFO *pMsgGinInfo);//发送消息至详细报文分析界面
};

#endif // LINKBRIEFTABLEVIEW_H
