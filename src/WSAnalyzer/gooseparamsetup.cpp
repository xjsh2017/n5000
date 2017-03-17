#ifdef OS_WINDOWS
#pragma execution_character_set("UTF-8")
#endif
#include "gooseparamsetup.h"
#include "ui_gooseparamsetup.h"
#include "IEC61850Analyzer.h"
#include "wssysconfig.h"
#include <QPushButton>
GooseParamSetup::GooseParamSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GooseParamSetup)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    initGooseCfg();
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(btnAccepted()));
}

GooseParamSetup::~GooseParamSetup()
{
    delete ui;
}
/**
 * @brief GooseParamSetup::initGooseCfg 从配置文件tsanaloccfg.ini获取GOOSE配置信息
 */
void GooseParamSetup::initGooseCfg()
{
    m_check_appid_zero = FALSE;
    m_check_confrev_diff_cfg = FALSE;
    m_check_daentries_diff_danum  = FALSE;
    m_check_danum_diff_cfg  = FALSE;
    m_check_datype_diff_cfg = FALSE;
    m_check_delay           = FALSE;
    m_check_drop_pkt = FALSE;
    m_check_double = FALSE;
    m_check_repeat = FALSE;
    m_check_dsname_diff_cfg = FALSE;
    m_check_dstmac_err = FALSE;
    m_check_gocbref_diff_cfg = FALSE;
    m_check_goid_diff_cfg = FALSE;
    m_check_goose_restart = FALSE;
    m_check_mac_diff_cfg = FALSE;
    m_check_ndscomm_true = FALSE;
    m_check_sqnum_jump = FALSE;
    m_check_sqnum_reverse = FALSE;
    m_check_sqnum_unreset = FALSE;
    m_check_stnum_chg_fake = FALSE;
    m_check_stnum_jump = FALSE;
    m_check_stnum_reverse = FALSE;
    m_check_t_error = FALSE;
    m_check_tal_zero = FALSE;
    m_check_test  = FALSE;
    m_check_tremble = FALSE;
    m_check_stnum_chg_right = FALSE;
    m_edit_delay = 0;
    m_edit_droppkt = 0;
    m_edit_t_error = 0;
    m_edit_tremble_minpercent = 0;
    CIEC61850Analyzer * iecAnalyzer = CIEC61850Analyzer::getInstance();
    GOOSE_SETTING_STRUCT* m_pgoose_setting = iecAnalyzer->m_gooseAnalyzer.GET_GOOSEANALYZE_SETTING();
    m_edit_delay = m_pgoose_setting->ndelay_T0_minpercent;
    m_edit_droppkt = m_pgoose_setting->ndrop_T0_minpercent;
    m_edit_t_error = m_pgoose_setting->nTerr_T_minpercent;
    m_edit_tremble_minpercent = m_pgoose_setting->ntremble_T0_minpercent;
    ui->lineEditDelay->setText(QString::number(m_edit_delay,10));
    ui->lineEditDroppkt->setText(QString::number(m_edit_droppkt,10));
    ui->lineEditTError->setText(QString::number(m_edit_t_error,10));
    ui->lineEditTrembleMinpercent->setText(QString::number(m_edit_tremble_minpercent,10));
    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_SQNUM_JUMP) == SETTING_GOOSE_SQNUM_JUMP)
    {
        m_check_sqnum_jump = TRUE;//sqNum跳变
        ui->checkBoxSqNumJump->setChecked(m_check_sqnum_jump);
    }

    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_SQNUM_REVERSE) ==SETTING_GOOSE_SQNUM_REVERSE)
    {
        m_check_sqnum_reverse= TRUE;//sqNum逆转
        ui->checkBoxSqNumReverse->setChecked(m_check_sqnum_reverse);
    }

    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_STNUM_JUMP) ==SETTING_GOOSE_STNUM_JUMP)
    {
        m_check_stnum_jump= TRUE;//stNum跳变
        ui->checkBoxStNumJump->setChecked(m_check_stnum_jump);
    }

    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_STNUM_REVERSE) == SETTING_GOOSE_STNUM_REVERSE)
    {
        m_check_stnum_reverse= TRUE;//stNum逆转
        ui->checkBoxStNumReverse->setChecked(m_check_stnum_reverse);
    }

    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_SQNUM_UNRESET) ==SETTING_GOOSE_SQNUM_UNRESET)
    {
        m_check_sqnum_unreset= TRUE;//stNum变化,sqNum未复归
        ui->checkBoxSqNumUnreset->setChecked(m_check_sqnum_unreset);
    }

    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_GOOSE_DOUBLE) ==SETTING_GOOSE_GOOSE_DOUBLE)
    {
        m_check_double= TRUE;//重复帧
        ui->checkBoxDouble->setChecked(m_check_double);
    }

    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_GOOSE_REPEAT) ==SETTING_GOOSE_GOOSE_REPEAT)
    {
        m_check_repeat= TRUE;//重复帧
        ui->checkBoxRepeat->setChecked(m_check_repeat);
    }

    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_STNUMCHANGE_FAKE) == SETTING_GOOSE_STNUMCHANGE_FAKE)
    {
        m_check_stnum_chg_fake= TRUE;//stNum虚变
        ui->checkBoxStNumChgFake->setChecked(m_check_stnum_chg_fake);
    }

    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_GOOSE_DELAY) == SETTING_GOOSE_GOOSE_DELAY)
    {
        m_check_delay= TRUE;//报文延迟
        ui->checkBoxDelay->setChecked(m_check_delay);
    }

    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_TAL_ZERO) ==  SETTING_GOOSE_TAL_ZERO)
    {
        m_check_tal_zero= TRUE;//TAL为零
        ui->checkBoxTalZero->setChecked(m_check_tal_zero);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_APPID_ZERO) == SETTING_GOOSE_APPID_ZERO)
    {
        m_check_appid_zero= TRUE;//APPID为零
        ui->checkBoxAppidZero->setChecked(m_check_appid_zero);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_NDSCOMM_TRUE) == SETTING_GOOSE_NDSCOMM_TRUE)
    {
        m_check_ndscomm_true= TRUE;//控制块需要配置
        ui->checkBoxNdscommTrue->setChecked(m_check_ndscomm_true);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_T0_T3_ERR) == SETTING_GOOSE_T0_T3_ERR)
    {
        m_check_t_error= TRUE;//发送机制错误
        ui->checkBoxTError->setChecked(m_check_t_error);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_DANUM_DIFFER_DSMEMBERNUM) == SETTING_GOOSE_DANUM_DIFFER_DSMEMBERNUM)
    {
        m_check_danum_diff_cfg= TRUE;//通道个数与配置不一致
        ui->checkBoxDaNumDiffCfg->setChecked(m_check_danum_diff_cfg);
    }

    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_GOCBREF_DIFFER_CFG) == SETTING_GOOSE_GOCBREF_DIFFER_CFG)
    {
        m_check_gocbref_diff_cfg= TRUE;//gocbref与配置不一致
        ui->checkBoxGocbrefDiffCfg->setChecked(m_check_gocbref_diff_cfg);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_GOID_DIFFER_CFG) == SETTING_GOOSE_GOID_DIFFER_CFG)
    {
        m_check_goid_diff_cfg= TRUE;//goid与配置不一致
        ui->checkBoxGoidDiffCfg->setChecked(m_check_goid_diff_cfg);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_DS_DIFFER_CFG) == SETTING_GOOSE_DS_DIFFER_CFG)
    {
        m_check_dsname_diff_cfg= TRUE;//dsname与配置不一致
        ui->checkBoxDsnameDiff->setChecked(m_check_dsname_diff_cfg);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_CONFREV_DIFFER_CFG) == SETTING_GOOSE_CONFREV_DIFFER_CFG)
    {
        m_check_confrev_diff_cfg= TRUE;//版本号与配置不一致
        ui->checkBoxConfrevDiffCfg->setChecked(m_check_confrev_diff_cfg);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_DAENTRIES_DIFFER_DANUM) == SETTING_GOOSE_DAENTRIES_DIFFER_DANUM)
    {
        m_check_daentries_diff_danum= TRUE;//numDataEntries与实际数据个数不一致
        ui->checkBoxDaentriesDiffDanum->setChecked(m_check_daentries_diff_danum);
    }

    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_DESTMAC_ERR) == SETTING_GOOSE_DESTMAC_ERR)
    {
        m_check_dstmac_err= TRUE;//目的MAC地址错误
        ui->checkBoxDstMacErr->setChecked(m_check_dstmac_err);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_GOOSE_RESTART) == SETTING_GOOSE_GOOSE_RESTART)
    {
        m_check_goose_restart= TRUE;//GOOSE链路重启
        ui->checkBoxRestart->setChecked(m_check_goose_restart);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_STNUMCHANGE_RIGHT) == SETTING_GOOSE_STNUMCHANGE_RIGHT)
    {
        m_check_stnum_chg_right= TRUE;//GOOSE变位
        ui->checkBoxStNumChgRight->setChecked(m_check_stnum_chg_right);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_TEST_TRUE) == SETTING_GOOSE_TEST_TRUE)
    {
        m_check_test= TRUE;//置检修标志
        ui->checkBoxTest->setChecked(m_check_test);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_MAC_DIFFER_CFG) == SETTING_GOOSE_MAC_DIFFER_CFG)
    {
        m_check_mac_diff_cfg= TRUE;//目的MAC地址与配置不一致
        ui->checkBoxMacDiffCfg->setChecked(m_check_mac_diff_cfg);
    }

    if((m_pgoose_setting->nerr_switch_setting & SETTING_GOOSE_DATYPE_DIFFER_CFG) == SETTING_GOOSE_DATYPE_DIFFER_CFG)
    {
        m_check_datype_diff_cfg= TRUE;//数据类型与配置文件不一致
        ui->checkBoxDatypeDiffCfg->setChecked(m_check_datype_diff_cfg);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_GOOSE_DROP) == SETTING_GOOSE_GOOSE_DROP)//丢帧
    {
        m_check_drop_pkt= TRUE;
        ui->checkBoxDropPkt->setChecked(m_check_drop_pkt);
    }

    if((m_pgoose_setting->nerr_switch_setting &  SETTING_GOOSE_GOOSE_THEMBLE) == SETTING_GOOSE_GOOSE_THEMBLE)//心跳报文抖动
    {
        m_check_tremble = TRUE;
        ui->checkBoxTremble->setChecked(m_check_tremble);
    }
}

void GooseParamSetup::btnAccepted()
{
    saveGooseCfg();
}
/**
 * @brief GooseParamSetup::saveGooseCfg 从界面保存配置至配置文件tsanaloccfg.ini
 */
void GooseParamSetup::saveGooseCfg()
{
    CIEC61850Analyzer * iecAnalyzer = CIEC61850Analyzer::getInstance();
    GOOSE_SETTING_STRUCT* m_pgoose_setting = iecAnalyzer->m_gooseAnalyzer.GET_GOOSEANALYZE_SETTING();
    m_pgoose_setting->nerr_switch_setting = 0;//复位
    if(ui->checkBoxSqNumJump->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_SQNUM_JUMP;   //sqNum跳变
    if(ui->checkBoxSqNumReverse->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_SQNUM_REVERSE;//sqNum逆转
    if(ui->checkBoxStNumJump->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_STNUM_JUMP;//stNum跳变
    if(ui->checkBoxStNumReverse->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_STNUM_REVERSE;//stNum逆转
    if(ui->checkBoxSqNumUnreset->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_SQNUM_UNRESET;//stNum变化,sqNum未复归
    if(ui->checkBoxDouble->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_DOUBLE; //重复帧
    if(ui->checkBoxRepeat->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_REPEAT;//内容完全重复
    if(ui->checkBoxStNumChgFake)
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_STNUMCHANGE_FAKE;//stNum虚变
    if(ui->checkBoxDelay->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_DELAY;//报文延迟
    if(ui->checkBoxTalZero->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_TAL_ZERO;//TAL为零
    if(ui->checkBoxAppidZero->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_APPID_ZERO;//APPID为零
    if(ui->checkBoxNdscommTrue->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_NDSCOMM_TRUE;//控制块需要配置
    if(ui->checkBoxTError->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_T0_T3_ERR;//发送机制错误
    if(ui->checkBoxDaNumDiffCfg->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DANUM_DIFFER_DSMEMBERNUM;//通道个数与配置不一致
    if(ui->checkBoxGocbrefDiffCfg->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOCBREF_DIFFER_CFG;//gocbref与配置不一致
    if(ui->checkBoxGoidDiffCfg->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOID_DIFFER_CFG;//goid与配置不一致
    if(ui->checkBoxDsnameDiff->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DS_DIFFER_CFG;//dsname与配置不一致
    if(ui->checkBoxConfrevDiffCfg->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_CONFREV_DIFFER_CFG;//版本号与配置不一致
    if(ui->checkBoxDaentriesDiffDanum->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DAENTRIES_DIFFER_DANUM;//numDataEntries与实际数据个数不一致
    if(ui->checkBoxDstMacErr->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DESTMAC_ERR;//目的MAC地址错误
    if(ui->checkBoxRestart->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_RESTART;//GOOSE链路重启
    if(ui->checkBoxStNumChgRight->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_STNUMCHANGE_RIGHT;//GOOSE变位
    if(ui->checkBoxTest->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_TEST_TRUE;//置检修标志
    if(ui->checkBoxMacDiffCfg->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_MAC_DIFFER_CFG;//目的MAC地址与配置不一致
    if(ui->checkBoxDatypeDiffCfg->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_DATYPE_DIFFER_CFG;//数据类型与配置文件不一致
    if(ui->checkBoxDropPkt->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_DROP;//丢帧
    if(ui->checkBoxTremble->isChecked())
        m_pgoose_setting->nerr_switch_setting |= SETTING_GOOSE_GOOSE_THEMBLE;//心跳报文抖动
    m_pgoose_setting->ndelay_T0_minpercent = ui->lineEditDelay->text().toInt();
    m_pgoose_setting->ndrop_T0_minpercent  = ui->lineEditDroppkt->text().toInt();
    m_pgoose_setting->nTerr_T_minpercent   = ui->lineEditTError->text().toInt();
    m_pgoose_setting->ntremble_T0_minpercent = ui->lineEditTrembleMinpercent->text().toInt();

    iecAnalyzer->m_gooseAnalyzer.I_GOOSEANALYZE_SET_SETTING();
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    //保存到系统
    wsSysconfig->m_wsIec61850AnaCfg.n_delay_T0_minpercent_goose   = m_pgoose_setting->ndelay_T0_minpercent;
    wsSysconfig->m_wsIec61850AnaCfg.n_drop_T0_minpercent_goose    = m_pgoose_setting->ndrop_T0_minpercent;
    wsSysconfig->m_wsIec61850AnaCfg.n_Terr_T_minpercent_goose     = m_pgoose_setting->nTerr_T_minpercent;
    wsSysconfig->m_wsIec61850AnaCfg.n_tremble_T0_minpercent_goose = m_pgoose_setting->ntremble_T0_minpercent;
    wsSysconfig->m_wsIec61850AnaCfg.n_err_switch_setting_goose    = m_pgoose_setting->nerr_switch_setting;
    wsSysconfig->saveconfig_goose();//保存配置文件
}
