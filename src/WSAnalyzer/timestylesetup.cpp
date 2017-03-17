#ifdef OS_WINDOWS
#pragma execution_character_set("UTF-8")
#endif
#include "timestylesetup.h"
#include "ui_timestylesetup.h"
#include "wssysconfig.h"
#include <QDebug>
#include <QPushButton>
QButtonGroup *btnRadioGroup;
TimeStyleSetup::TimeStyleSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeStyleSetup)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    m_nradTimeType = 3;
    connect(ui->radioButton,SIGNAL(clicked()),this,SLOT(onRadioClicked()));
    connect(ui->radioButton_2,SIGNAL(clicked()),this,SLOT(onRadioClicked()));
    connect(ui->radioButton_3,SIGNAL(clicked()),this,SLOT(onRadioClicked()));
    connect(ui->radioButton_4,SIGNAL(clicked()),this,SLOT(onRadioClicked()));
    connect(ui->radioButton_5,SIGNAL(clicked()),this,SLOT(onRadioClicked()));

    btnRadioGroup = new QButtonGroup(this);
    btnRadioGroup->addButton(ui->radioButton, 0);
    btnRadioGroup->addButton(ui->radioButton_2, 1);
    btnRadioGroup->addButton(ui->radioButton_3, 2);
    btnRadioGroup->addButton(ui->radioButton_4, 3);
    btnRadioGroup->addButton(ui->radioButton_5, 4);

    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    btnRadioGroup->buttonClicked(ui->radioButton);
    switch (wsSysconfig->m_wsShowStyle.nDataViewTimeStyle) {
    case 0:
        ui->radioButton->setChecked(true);
        break;
    case 1:
        ui->radioButton_2->setChecked(true);
        break;
    case 2:
        ui->radioButton_3->setChecked(true);
        break;
    case 3:
        ui->radioButton_4->setChecked(true);
        break;
    case 4:
        ui->radioButton_5->setChecked(true);
        break;
    default:
        break;
    }
    m_nradTimeType = wsSysconfig->m_wsShowStyle.nDataViewTimeStyle;

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(btnAccepted()));
}

TimeStyleSetup::~TimeStyleSetup()
{
    delete ui;
}
/**
 * @brief TimeStyleSetup::onRadioClicked 切换时间格式
 */
void TimeStyleSetup::onRadioClicked()
{
    m_nradTimeType = btnRadioGroup->checkedId();
}
/**
 * @brief TimeStyleSetup::btnAccepted 确认保存时间格式配置，并写入配置文件
 */
void TimeStyleSetup::btnAccepted()
{
    //qDebug() << m_nradTimeType;
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    wsSysconfig->m_wsShowStyle.nDataViewTimeStyle = m_nradTimeType;
    wsSysconfig->saveconfig_common();//保存配置到文件
}




