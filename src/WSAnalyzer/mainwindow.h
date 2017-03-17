#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mdichild.h"
class MdiChild;
class QMdiSubWindow;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool openFile(const QString &fileName);
    //初始化配置管理器
    int InitCfgMngr();

private:
    Ui::MainWindow *ui;
    bool loadFile(const QString &fileName);
    QMdiSubWindow *findMdiChild(const QString &fileName) const;
    bool isScdLoading;
    QString m_strScdFileFullPathName;//SCD文件全路径
    QString m_strPcapFileFullPathName;//PCAP文件全路径
    void fillMultiList(MAP_TS_LIBCAP_FILE* pmap_tslibcapfilelist);
    MAP_TS_LIBCAP_FILE map_filelist;
public:
    int loadScdFile();
    void loadScdInform();

protected slots:
    void open();
    void setupTimeStyle();
    void setupGooseParam();
    void setupSmvParam();
    void setupMmsParam();
    void statisticalReport();
    void saveAs();
    void loadScd();
    void openMultiFiles();
    void openedFiles();

private slots:
    MdiChild *createMdiChild();
signals:
    void connectLinkBriefTable();
    void loadScdSuccess();
};

#endif // MAINWINDOW_H
