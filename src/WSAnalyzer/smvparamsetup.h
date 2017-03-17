#ifndef SMVPARAMSETUP_H
#define SMVPARAMSETUP_H

#include <QDialog>

namespace Ui {
class SmvParamSetup;
}

class SmvParamSetup : public QDialog
{
    Q_OBJECT

public:
    explicit SmvParamSetup(QWidget *parent = 0);
    ~SmvParamSetup();

private:
    Ui::SmvParamSetup *ui;
    bool m_check_appidzero;
    bool m_check_asdu_dif_cfg;
    bool m_check_asynchronism;
    bool m_check_cnt_jump;
    bool m_check_cnt_reverse;
    bool m_check_2da_data_dif;
    bool m_check_danum_dif_cfg;
    bool m_check_data_overflow;
    bool m_check_delay;
    bool m_check_delay_trtg_chg;
    bool m_check_doublesv;
    bool m_check_drop_pkt;
    bool m_check_dstmac_err;
    bool m_check_mac_dif_cfg;
    bool m_check_pdulen_zero;
    bool m_check_quality_chg;
    bool m_check_rate_unstable;
    bool m_check_svid_dif_cfg;
    bool m_check_test;
    bool m_check_tremble;
    bool m_check_confrev_dif_cfg;
    bool m_check_dsname_diff_cfg;
    int m_edit_delay;
    int m_edit_droppkt;
    int m_edit_tremble;
    void initSmvCfg();
    void saveSmvCfg();
private slots:
    void btnAccepted();
};

#endif // SMVPARAMSETUP_H
