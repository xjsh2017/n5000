#ifndef GOOSEPARAMSETUP_H
#define GOOSEPARAMSETUP_H

#include <QDialog>

namespace Ui {
class GooseParamSetup;
}

class GooseParamSetup : public QDialog
{
    Q_OBJECT

public:
    explicit GooseParamSetup(QWidget *parent = 0);
    ~GooseParamSetup();

private:
    Ui::GooseParamSetup *ui;    
    bool m_check_appid_zero;
    bool m_check_confrev_diff_cfg;
    bool m_check_daentries_diff_danum;
    bool m_check_danum_diff_cfg;
    bool m_check_datype_diff_cfg;
    bool m_check_delay;
    bool m_check_drop_pkt;
    bool m_check_double;
    bool m_check_repeat;
    bool m_check_dsname_diff_cfg;
    bool m_check_dstmac_err;
    bool m_check_gocbref_diff_cfg;
    bool m_check_goid_diff_cfg;
    bool m_check_goose_restart;
    bool m_check_mac_diff_cfg;
    bool m_check_ndscomm_true;
    bool m_check_sqnum_jump;
    bool m_check_sqnum_reverse;
    bool m_check_sqnum_unreset;
    bool m_check_stnum_chg_fake;
    bool m_check_stnum_jump;
    bool m_check_stnum_reverse;
    bool m_check_t_error;
    bool m_check_tal_zero;
    bool m_check_test;
    bool m_check_tremble;
    bool m_check_stnum_chg_right;
    int m_edit_delay;
    int m_edit_droppkt;
    int m_edit_t_error;
    int m_edit_tremble_minpercent;
    void initGooseCfg();
    void saveGooseCfg();
private slots:
    void btnAccepted();
};

#endif // GOOSEPARAMSETUP_H
