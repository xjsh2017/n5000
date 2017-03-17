#ifndef DLGPKTFILTERCONDITION_H
#define DLGPKTFILTERCONDITION_H

#include <QDialog>
#include "capanalyzerstruct.h"
namespace Ui {
class dlgPktFilterCondition;
}

class dlgPktFilterCondition : public QDialog
{
    Q_OBJECT

public:
    explicit dlgPktFilterCondition(QWidget *parent = 0);
    ~dlgPktFilterCondition();
    PACKETQUER_FILTER *m_pQuery_Filter;

private:
    Ui::dlgPktFilterCondition *ui;
    void initFilter();
    int saveConfig();
    void accept();
private slots:
    void proxyChange(int num);
};

#endif // DLGPKTFILTERCONDITION_H
