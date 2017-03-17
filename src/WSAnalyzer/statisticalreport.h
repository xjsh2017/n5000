#ifndef STATISTICALREPORT_H
#define STATISTICALREPORT_H

#include <QDialog>
#include "cappackagesmnger.h"
namespace Ui {
class StatisticalReport;
}

class StatisticalReport : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticalReport(QWidget *parent = 0);
    ~StatisticalReport();

private:
    Ui::StatisticalReport *ui;
    QString m_strTile;

public:
    CapPackagesMnger* pcappackagesmnger;
    void initReport(QString dir);//初始化报表
private slots:
    void saveFileToXls();//保存报表
};

#endif // STATISTICALREPORT_H
