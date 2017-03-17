#ifdef OS_WINDOWS
#pragma execution_character_set("UTF-8")
#endif
#include "mmsparamsetup.h"
#include "ui_mmsparamsetup.h"
#include "wssysconfig.h"
#include <QPushButton>
MmsParamSetup::MmsParamSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MmsParamSetup)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    ui->lineEdit->setToolTip("填写说明：多个数据集名称之间用$分隔！");
    cfg2face();
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(btnAccepted()));
}

MmsParamSetup::~MmsParamSetup()
{
    delete ui;
}
/**
 * @brief MmsParamSetup::face2cfg 从界面保存配置至配置文件tsanaloccfg.ini
 * @return
 */
int MmsParamSetup::face2cfg()
{
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    wsSysconfig->m_wsIec61850AnaCfg.breortexceptana_mms = m_bcheckfiltermmsrpt;
    wsSysconfig->m_wsIec61850AnaCfg.n_excepcodition_mms= 1;
    memset(wsSysconfig->m_wsIec61850AnaCfg.c_exceptcodition_mms,0,sizeof(wsSysconfig->m_wsIec61850AnaCfg.c_exceptcodition_mms));
    char dsexceptlsit[2048];
    memset(dsexceptlsit,0,2048);
    m_stredit_dslist = ui->lineEdit->text();
    int nlength = m_stredit_dslist.length();
    if(nlength > 2048)
        nlength = 2048;
    int ntemp = 0;
    memcpy(dsexceptlsit,m_stredit_dslist.toStdString().c_str(),nlength);
    for(int i = 0; i < nlength;i++)
    {
        if(dsexceptlsit[i] != '$')
        {
            memcpy(wsSysconfig->m_wsIec61850AnaCfg.c_exceptcodition_mms[wsSysconfig->m_wsIec61850AnaCfg.n_excepcodition_mms-1]+ntemp,dsexceptlsit+i,1);
            ntemp ++;
            if(ntemp == 65)//越限后强制到下一节点
            {
                ntemp = 0;
                wsSysconfig->m_wsIec61850AnaCfg.n_excepcodition_mms ++;//条件增加
                break;
            }
        }
        else
        {
            ntemp = 0;
            wsSysconfig->m_wsIec61850AnaCfg.n_excepcodition_mms ++;//条件增加
            if(wsSysconfig->m_wsIec61850AnaCfg.n_excepcodition_mms > 128)
                break;
        }
    }
    wsSysconfig->saveconfig_mms();
    return 0;
}
/**
 * @brief MmsParamSetup::cfg2face 从配置文件tsanaloccfg.ini读取MMS初始配置至界面
 * @return
 */
int MmsParamSetup::cfg2face()
{
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    QString strTemp;
    m_bcheckfiltermmsrpt = wsSysconfig->m_wsIec61850AnaCfg.breortexceptana_mms;
    if(m_bcheckfiltermmsrpt)
    {
        ui->checkBox->setChecked(true);
    }
    for(int i = 0; i < wsSysconfig->m_wsIec61850AnaCfg.n_excepcodition_mms; i++)
    {
        strTemp.sprintf("%s",wsSysconfig->m_wsIec61850AnaCfg.c_exceptcodition_mms[i]);
        if(m_stredit_dslist.isEmpty())
        {
            m_stredit_dslist = strTemp;
        }
        else
        {
            m_stredit_dslist = m_stredit_dslist + "$" + strTemp;
        }
    }
    if(!m_stredit_dslist.isEmpty())
        ui->lineEdit->setText(m_stredit_dslist);
    return 0;
}
/**
 * @brief MmsParamSetup::btnAccepted 保存配置确认
 */
void MmsParamSetup::btnAccepted()
{
    face2cfg();
}
