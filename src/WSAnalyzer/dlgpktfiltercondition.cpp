#ifdef OS_WINDOWS
#pragma execution_character_set("UTF-8")
#endif
#include "dlgpktfiltercondition.h"
#include "ui_dlgpktfiltercondition.h"
#include "Keywords61850tranformer.h"
#include <QMessageBox>
#include <QPushButton>
extern CKeywords61850tranformer *keywords61850Ts;
dlgPktFilterCondition::dlgPktFilterCondition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgPktFilterCondition)
{
    ui->setupUi(this);
    initFilter();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(proxyChange(int)));
}

dlgPktFilterCondition::~dlgPktFilterCondition()
{
    delete ui;
}
/**
 * @brief dlgPktFilterCondition::initFilter 初始化报文过滤配置
 */
void dlgPktFilterCondition::initFilter()
{
    ui->comboBox->addItem("none0", 0);
    ui->comboBox->addItem("none1", 1);
    ui->comboBox->addItem("none2", 2);
    ui->comboBox->addItem("none3", 3);
    ui->comboBox->addItem("none4", 4);
    ui->comboBox->addItem("none5", 5);
    ui->comboBox->addItem("none6", 6);
    ui->comboBox->setItemText(0, tr("发送IP地址模糊匹配"));
    ui->comboBox->setItemText(1, tr("目的IP地址模糊匹配"));
    ui->comboBox->setItemText(2, tr("发送和目的IP地址模糊匹配"));
    ui->comboBox->setItemText(3, tr("发送MAC地址模糊匹配"));
    ui->comboBox->setItemText(4, tr("目的MAC地址模糊匹配"));
    ui->comboBox->setItemText(5, tr("发送端和目的MAC地址模糊匹配"));
    ui->comboBox->setItemText(6, tr("根据ACSI报文类型查询"));
}
/**
 * @brief dlgPktFilterCondition::proxyChange 过滤选择切换监控
 * @param num
 */
void dlgPktFilterCondition::proxyChange(int num)
{
    if(num == 6)
    {
        int nindex = 0;
        QString str;
        std::map<int, std::string>::iterator iter;
        for(iter = keywords61850Ts->m_mapEnumAcsiType.begin(); iter != keywords61850Ts->m_mapEnumAcsiType.end(); iter ++ )
        {
            if(iter->first == 0)//过滤掉未知服务
                continue;
            str.sprintf("%s",QString::fromLocal8Bit(iter->second.c_str()).toStdString().c_str());
            ui->comboBox_2->addItem("none0", nindex);
            ui->comboBox_2->setItemText(nindex, str);
            nindex++;
        }
        ui->lineEdit->setEnabled(false);
    }
    else
    {
       ui->comboBox_2->clear();
       ui->lineEdit->setEnabled(true);
    }
}
/**
 * @brief dlgPktFilterCondition::saveConfig 保存过滤配置
 * @return
 */
int dlgPktFilterCondition::saveConfig()
{
    int nresult = 0;
    //开始输入
    int nindex;
    nindex = ui->comboBox->currentIndex();
    if(nindex > -1)
        m_pQuery_Filter->nfilter_type = nindex;
    nindex = ui->comboBox_2->currentIndex();
    if(nindex > -1)
        m_pQuery_Filter->nfliter_apptype = nindex + 1;
    #ifdef OS_WINDOWS
    _snprintf_s(m_pQuery_Filter->c_filter_appword,127,"%s",(const char*)ui->lineEdit->text().toStdString().c_str());
    #else
    snprintf(m_pQuery_Filter->c_filter_appword,127,"%s",(const char*)ui->lineEdit->text().toStdString().c_str());
    #endif

   if(m_pQuery_Filter->nfilter_type != 6 && strlen(m_pQuery_Filter->c_filter_appword) == 0)
   {
       QMessageBox::information(this,"提示","请输入过滤条件");
       nresult = -1;
   }
   return nresult;
}
/**
 * @brief dlgPktFilterCondition::accept 重写
 */
void dlgPktFilterCondition::accept()
{

    if(saveConfig() == -1)
    {
        return;
    }else
    {
        this->done(1);;
    }
}

