#ifndef MDICHILD_H
#define MDICHILD_H

#include <QWidget>
#include "ui_mdichild.h"
#include "cappackagesmnger.h"
#include "anakit.h"
#include "linkbrieftableview.h"
namespace Ui {
class MdiChild;
}

class MdiChild : public QWidget
{
    Q_OBJECT

public:
    explicit MdiChild(QWidget *parent = 0);
    ~MdiChild();
    bool loadFile(const QString &fileName);
    QString currentFile() { return curFile; }

private:
    Ui::MdiChild *ui;
    void setCurrentFile(const QString &fileName);
    QString curFile;
    QTreeWidgetItem *itemRoot;//链路概要信息根节点
    QTreeWidgetItem *itemLeafMms;
    QTreeWidgetItem *itemLeafGs;
    QTreeWidgetItem *itemLeafSmv;
    QTreeWidgetItem *itemLeaf1588;
    QTreeWidgetItem *itemLeafUdp;
    QTreeWidgetItem *itemLeafTcp;
    QTreeWidgetItem *itemLeafArp;
    QTreeWidgetItem *itemLeafOther;
    QTreeWidgetItem *itemDetailedRoot;//详细报文解析根节点
public:
    AnaKit *m_anakit;
    linkBriefTableView *linkView;

public:
    void fillLinkTree();
    void fillNetConnectInfo(CAPCONNECTINFO *pcapconnectinfo);
    QTreeWidgetItem *GetParentByEtherType(int nEtherType);
    QStringList GetConnectDesc(CAPCONNECTINFO *pcapconnectinfo);
    public:
    void test();
private slots:
    void OnMySelectTreeItem(QTreeWidgetItem * item, int column);
signals:
    void wsWriteLinkMsgDetail(CapPackagesMnger *m_cappackagesmnger,CAPCONNECTINFO *getSelectCapConnectInfo,int nDataShowtype,int linkType);
    void wsUpdateWSAnalData(CapPackagesMnger *m_cappackagesmnger,CAPCONNECTINFO *getSelectCapConnectInfo,int nDataShowtype,int linkType);
};

Q_DECLARE_METATYPE( CAPCONNECTINFO)//如果要使自定义类型或其他非QMetaType内置类型在QVariant中使用，必须使用该宏
#endif // MDICHILD_H
