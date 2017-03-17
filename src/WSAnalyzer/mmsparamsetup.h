#ifndef MMSPARAMSETUP_H
#define MMSPARAMSETUP_H

#include <QDialog>

namespace Ui {
class MmsParamSetup;
}

class MmsParamSetup : public QDialog
{
    Q_OBJECT

public:
    explicit MmsParamSetup(QWidget *parent = 0);
    ~MmsParamSetup();

private:
    Ui::MmsParamSetup *ui;
    int face2cfg();//从界面保存配置至配置文件
    int cfg2face();//从配置文件读取至界面
    QString m_stredit_dslist;//数据集名称
    bool m_bcheckfiltermmsrpt;//模拟量报告不按事件统计
private slots:
    void btnAccepted();
};

#endif // MMSPARAMSETUP_H
