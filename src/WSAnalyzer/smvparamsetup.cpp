#ifdef OS_WINDOWS
#pragma execution_character_set("UTF-8")
#endif
#include "smvparamsetup.h"
#include "IEC61850Analyzer.h"
#include "wssysconfig.h"
#include "ui_smvparamsetup.h"
#include <QPushButton>
SmvParamSetup::SmvParamSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SmvParamSetup)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    initSmvCfg();
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(btnAccepted()));
}

SmvParamSetup::~SmvParamSetup()
{
    delete ui;
}
/**
 * @brief SmvParamSetup::initSmvCfg 从tsanaloccfg.ini读取SMV配置至界面
 */
void SmvParamSetup::initSmvCfg()
{
    m_check_appidzero     = false;
    m_check_asdu_dif_cfg     = false;
    m_check_asynchronism     = false;
    m_check_cnt_jump     = false;
    m_check_cnt_reverse     = false;
    m_check_2da_data_dif     = false;
    m_check_danum_dif_cfg     = false;
    m_check_data_overflow     = false;
    m_check_delay     = false;
    m_check_delay_trtg_chg     = false;
    m_check_doublesv     = false;
    m_check_drop_pkt     = false;
    m_check_dstmac_err     = false;
    m_check_mac_dif_cfg     = false;
    m_check_pdulen_zero     = false;
    m_check_quality_chg     = false;
    m_check_rate_unstable     = false;
    m_check_svid_dif_cfg     = false;
    m_check_test     = false;
    m_check_tremble     = false;
    m_check_confrev_dif_cfg     = false;
    m_check_dsname_diff_cfg     = false;
    CIEC61850Analyzer * iecAnalyzer = CIEC61850Analyzer::getInstance();
    SMV_SETTING_STRUCT* psetting = iecAnalyzer->m_smvAnalyzer.GET_SMVANALYZE_SETTING();
    if ( (psetting->nerr_switch_setting & SETTING_SMV_DELAY) == SETTING_SMV_DELAY )//延迟
    {
        m_check_delay     = true;
        ui->checkBoxDelay->setChecked(m_check_delay);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_APPID_ZERO) == SETTING_SMV_APPID_ZERO )//APPID为零
    {
        m_check_appidzero     = true;
        ui->checkBoxAppidZero->setChecked(m_check_appidzero);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_PDULEN_ZERO) == SETTING_SMV_PDULEN_ZERO )//PDU长度为零
    {
        m_check_pdulen_zero     = true;
        ui->checkBoxPduLenZero->setChecked(m_check_pdulen_zero);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_DESTMAC_ERR) == SETTING_SMV_DESTMAC_ERR )//目的MAC地址错误
    {
        m_check_dstmac_err     = true;
        ui->checkBoxDstMacErr->setChecked(m_check_dstmac_err);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_MAC_DIFFER_CFG) == SETTING_SMV_MAC_DIFFER_CFG )//目的MAC地址与配置不一致
    {
        m_check_mac_dif_cfg     = true;
        ui->checkBoxMacDifferCfg->setChecked(m_check_mac_dif_cfg);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_ASDU_DIFFER_CFG) == SETTING_SMV_ASDU_DIFFER_CFG )//ASDU个数与配置不一致
    {
        m_check_asdu_dif_cfg     = true;
        ui->checkBoxAsduDifferCfg->setChecked(m_check_asdu_dif_cfg);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_TREMBLE) == SETTING_SMV_TREMBLE )//抖动
    {
        m_check_tremble     = true;
        ui->checkBoxTremble->setChecked(m_check_tremble);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_RATE_UNSTABLE) == SETTING_SMV_RATE_UNSTABLE )//频率不稳定
    {
        m_check_rate_unstable     = true;
        ui->checkBoxRateUnstable->setChecked(m_check_rate_unstable);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_DOUBLE) == SETTING_SMV_DOUBLE )//双SMV
    {
        m_check_doublesv     = true;
        ui->checkBoxDouble->setChecked(m_check_doublesv);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_SMPCNT_JUMP) == SETTING_SMV_SMPCNT_JUMP )//序号跳变
    {
        m_check_cnt_jump     = true;
        ui->checkBoxCntJump->setChecked(m_check_cnt_jump);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_SMPCNT_REVERSE) == SETTING_SMV_SMPCNT_REVERSE )//序号逆转
    {
        m_check_cnt_reverse     = true;
        ui->checkBoxCntReverse->setChecked(m_check_cnt_reverse);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_ASYNCHRONISM) == SETTING_SMV_ASYNCHRONISM )//丢失同步信号
    {
        m_check_asynchronism     = true;
        ui->checkBoxAsynchronism->setChecked(m_check_asynchronism);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_DANUM_DIFFER_DSMEMBERNUM) == SETTING_SMV_DANUM_DIFFER_DSMEMBERNUM )//通道数与配置不一致
    {
        m_check_danum_dif_cfg     = true;
        ui->checkBoxDaNumDifferCfg->setChecked(m_check_danum_dif_cfg);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_DS_DIFFER_CFG) == SETTING_SMV_DS_DIFFER_CFG )//数据集与配置不一致
    {
        m_check_dsname_diff_cfg = true;
        ui->checkBoxDsNameDifferCfg->setChecked(m_check_dsname_diff_cfg);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_SVID_DIFFER_CFG) == SETTING_SMV_SVID_DIFFER_CFG )//svID与配置不一致
    {
        m_check_svid_dif_cfg     = true;
        ui->checkBoxSvidDifferCfg->setChecked(m_check_svid_dif_cfg);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_CONFREV_DIFFER_CFG) == SETTING_SMV_CONFREV_DIFFER_CFG )//confRev与配置不一致
    {
        m_check_confrev_dif_cfg     = true;
        ui->checkBoxConfrevDifferCfg->setChecked(m_check_confrev_dif_cfg);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_DROP_PKT) == SETTING_SMV_DROP_PKT )//丢帧
    {
        m_check_drop_pkt     = true;
        ui->checkBoxDropPtk->setChecked(m_check_drop_pkt);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_DELAY_TR_TG_CHANGE) == SETTING_SMV_DELAY_TR_TG_CHANGE )//通道延时变化
    {
        m_check_delay_trtg_chg     = true;
        ui->checkBoxDelayTrtgChange->setChecked(m_check_delay_trtg_chg);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_QUALITY_CHANGE) == SETTING_SMV_QUALITY_CHANGE )//SMV数据品质变化
    {
        m_check_quality_chg     = true;
        ui->checkBoxQualityChange->setChecked(m_check_quality_chg);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_DA_ERR) == SETTING_SMV_DA_ERR )//数据异常
    {
        m_check_data_overflow     = true;
        ui->checkBoxDataOverflow->setChecked(m_check_data_overflow);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_DOUBLE_AD_DATA_DIFFER) == SETTING_SMV_DOUBLE_AD_DATA_DIFFER )//双AD数据不一致
    {
        m_check_2da_data_dif     = true;
        ui->checkBox2AdDataDiffer->setChecked(m_check_2da_data_dif);
    }
    if ( (psetting->nerr_switch_setting & SETTING_SMV_TEST) == SETTING_SMV_TEST )//置检修标志
    {
        m_check_test     = true;
        ui->checkBoxTest->setChecked(m_check_test);
    }
    m_edit_delay   = psetting->ndelay_minpercent;
    m_edit_droppkt = psetting->ndrop_minpercent;
    m_edit_tremble = psetting->ntremble_minpercent;
    ui->lineEditDelayInpercent->setText(QString::number(m_edit_delay));
    ui->lineEditDropPktMinpercent->setText(QString::number(m_edit_droppkt));
    ui->lineEditTrembleMinpercent->setText(QString::number(m_edit_tremble));

}
/**
 * @brief SmvParamSetup::btnAccepted 确认保存配置按钮
 */
void SmvParamSetup::btnAccepted()
{
    saveSmvCfg();
}
/**
 * @brief SmvParamSetup::saveSmvCfg 从界面读取SMV配置保存至配置文件
 */
void SmvParamSetup::saveSmvCfg()
{
    CIEC61850Analyzer * iecAnalyzer = CIEC61850Analyzer::getInstance();
    SMV_SETTING_STRUCT* psetting = iecAnalyzer->m_smvAnalyzer.GET_SMVANALYZE_SETTING();
    psetting->nerr_switch_setting = 0;//复位
    if (ui->checkBoxDelay->isChecked() )//延迟
    {
        psetting->nerr_switch_setting|=SETTING_SMV_DELAY;
    }
    if ( ui->checkBoxAppidZero->isChecked())//APPID为零
    {
        psetting->nerr_switch_setting|=SETTING_SMV_APPID_ZERO;
    }
    if ( ui->checkBox2AdDataDiffer->isChecked())//双AD数据不一致
    {
        psetting->nerr_switch_setting|=SETTING_SMV_DOUBLE_AD_DATA_DIFFER;
    }
    if ( ui->checkBoxPduLenZero->isChecked() )//PDU长度为零
    {
        psetting->nerr_switch_setting |= SETTING_SMV_PDULEN_ZERO;
    }
    if ( ui->checkBoxDstMacErr->isChecked() )//目的MAC地址错误
    {
            psetting->nerr_switch_setting |= SETTING_SMV_DESTMAC_ERR;
    }
    if ( ui->checkBoxMacDifferCfg->isChecked())//目的MAC地址与配置不一致
    {

        psetting->nerr_switch_setting |= SETTING_SMV_MAC_DIFFER_CFG;
    }
    if (ui->checkBoxAsduDifferCfg->isChecked()  )//ASDU个数与配置不一致
    {
        psetting->nerr_switch_setting |= SETTING_SMV_ASDU_DIFFER_CFG;
    }
    if ( ui->checkBoxTremble->isChecked() )//抖动
    {
        psetting->nerr_switch_setting |=SETTING_SMV_TREMBLE;
    }
    if ( ui->checkBoxRateUnstable->isChecked() )//频率不稳定
    {
        psetting->nerr_switch_setting |= SETTING_SMV_RATE_UNSTABLE;
    }
    if ( ui->checkBoxDouble->isChecked() )//双SMV
    {
        psetting->nerr_switch_setting |= SETTING_SMV_DOUBLE;
    }
    if ( ui->checkBoxCntJump->isChecked())//序号跳变
    {
        psetting->nerr_switch_setting |= SETTING_SMV_SMPCNT_JUMP ;
    }
    if (ui->checkBoxCntReverse->isChecked() )//序号逆转
    {
        psetting->nerr_switch_setting |= SETTING_SMV_SMPCNT_REVERSE;
    }
    if (ui->checkBoxAsynchronism->isChecked()  )//丢失同步信号
    {
        psetting->nerr_switch_setting |= SETTING_SMV_ASYNCHRONISM;
    }
    if ( ui->checkBoxDaNumDifferCfg->isChecked() )//通道数与配置不一致
    {
        psetting->nerr_switch_setting |= SETTING_SMV_DANUM_DIFFER_DSMEMBERNUM;
    }
    if ( ui->checkBoxDsNameDifferCfg->isChecked() )//数据集与配置不一致
    {
        psetting->nerr_switch_setting|= SETTING_SMV_DS_DIFFER_CFG;
    }
    if ( ui->checkBoxSvidDifferCfg->isChecked() )//svID与配置不一致
    {
        psetting->nerr_switch_setting |= SETTING_SMV_SVID_DIFFER_CFG;
    }
    if (ui->checkBoxConfrevDifferCfg->isChecked())//confRev与配置不一致
    {
        psetting->nerr_switch_setting |= SETTING_SMV_CONFREV_DIFFER_CFG ;
    }
    if ( ui->checkBoxDropPtk->isChecked() )//丢帧
    {
        psetting->nerr_switch_setting |=SETTING_SMV_DROP_PKT;
    }
    if (ui->checkBoxDelayTrtgChange->isChecked()  )//通道延时变化
    {
        psetting->nerr_switch_setting |= SETTING_SMV_DELAY_TR_TG_CHANGE;
    }
    if ( ui->checkBoxQualityChange->isChecked() )//SMV数据品质变化
    {
        psetting->nerr_switch_setting |= SETTING_SMV_QUALITY_CHANGE;
    }
    if ( ui->checkBoxDataOverflow->isChecked())//数据异常
    {
        psetting->nerr_switch_setting |= SETTING_SMV_DA_ERR ;
    }

    if ( ui->checkBoxTest->isChecked() )//置检修标志
    {
        psetting->nerr_switch_setting |= SETTING_SMV_TEST;
    }
    psetting->ndelay_minpercent = ui->lineEditDelayInpercent->text().toInt();
    psetting->ndrop_minpercent  = ui->lineEditDropPktMinpercent->text().toInt();
    psetting->ntremble_minpercent = ui->lineEditTrembleMinpercent->text().toInt();
    iecAnalyzer->m_smvAnalyzer.I_SMVANALYZE_SET_SETTING();
    //保存到系统
    //开关
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    wsSysconfig->m_wsIec61850AnaCfg.n_err_switch_setting_sv = psetting->nerr_switch_setting;
    //帧间隔与250us的差X，250us*ndelay_minpercent <= X <250us*ndrop_minpercent，(且序号连续),则是采样值延迟
    wsSysconfig->m_wsIec61850AnaCfg.n_delay_minpercent_sv = psetting->ndelay_minpercent;
    //帧间隔减250us的绝对值，>=250us*ndrop_minpercent，且序号不连续，则是采样值丢帧
    wsSysconfig->m_wsIec61850AnaCfg.n_drop_minpercent_sv = psetting->ndrop_minpercent;
    //帧间隔减250us的绝对值X，250us*ntremble_minpercent < X < 250us*ndelay_minpercent，(且序号连续)，则是采样值抖动
    wsSysconfig->m_wsIec61850AnaCfg.n_tremble_minpercent_sv = psetting->ntremble_minpercent;
    wsSysconfig->saveconfig_sv();//保存到配置文件
}
