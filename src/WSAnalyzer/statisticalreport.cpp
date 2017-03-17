#ifdef OS_WINDOWS
#pragma execution_character_set("UTF-8")
#endif
#include "statisticalreport.h"
#include "ui_statisticalreport.h"
#include "wssysconfig.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
StatisticalReport::StatisticalReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticalReport)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView {background-color:lblue } QHeaderView::section {background-color:lightblue;color: black;padding-left: 4px;}");
    ui->tableWidget->setStyleSheet("QTableWidget::item:selected { background-color: rgb(255,192,203) }");
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this, SLOT(saveFileToXls()));
}

StatisticalReport::~StatisticalReport()
{
    delete ui;
}

/**
 * @brief StatisticalReport::initReport 初始化报文信息，数据源自cap文件总结构m_capparserninfo
 * @param dir 打开的PCAP文件路径
 */
void StatisticalReport::initReport(QString dir)
{
    QString strTempValue,strtemp;
    strtemp.sprintf("%s",pcappackagesmnger->m_capparserninfo.cparserfilename);
    strTempValue.sprintf("流量统计报告:%s",strtemp.toStdString().c_str());
    ui->labelDevName->setText(QString::fromLocal8Bit(pcappackagesmnger->m_capparserninfo.crecoriedname));
    ui->labelDevPort->setText(QString::fromStdString(pcappackagesmnger->m_capparserninfo.crecorportname));
    ui->labelStartTime->setText(QString::fromStdString(pcappackagesmnger->m_capparserninfo.cstarttimestamp));
    strTempValue.sprintf("%.6f 秒",pcappackagesmnger->m_capparserninfo.ftime_lastpk_2_first);
    ui->labelKeepTime->setText(strTempValue);
    strTempValue.sprintf("%u 字节",pcappackagesmnger->m_capparserninfo.lTotalCapLenth);
    ui->labelTotalByte->setText(strTempValue);
    strTempValue.sprintf("%d 帧",pcappackagesmnger->m_capparserninfo.napppackages);
    ui->labelTotalFrame->setText(strTempValue);
    strTempValue.sprintf("%d 帧",pcappackagesmnger->m_capparserninfo.napp92packages);
    ui->labelSMV->setText(strTempValue);
    strTempValue.sprintf("%d 帧",pcappackagesmnger->m_capparserninfo.nappgoosepackages);
    ui->labelGoose->setText(strTempValue);
    strTempValue.sprintf("%d 帧",pcappackagesmnger->m_capparserninfo.nappmmspackages);
    ui->labelMMS->setText(strTempValue);
    strTempValue.sprintf("%d 帧",pcappackagesmnger->m_capparserninfo.ntime1588packages);
    ui->labelPTP->setText(strTempValue);
    strTempValue.sprintf("%d 帧",pcappackagesmnger->m_capparserninfo.nappotherpackages);
    ui->labelOther->setText(strTempValue);
    double fFlow = pcappackagesmnger->m_capparserninfo.lTotalCapLenth * 8/ (pcappackagesmnger->m_capparserninfo.ftime_lastpk_2_first*1024*1024);
    strTempValue.sprintf("%.3f Mb/秒",fFlow);
    ui->labelFlowRate->setText(strTempValue);
    int nFrame =  (int)pcappackagesmnger->m_capparserninfo.napppackages/pcappackagesmnger->m_capparserninfo.ftime_lastpk_2_first;
    strTempValue.sprintf("%d 帧/秒",nFrame);
    ui->labelFrameRate->setText(strTempValue);

    std::map <int, CAPCONNECTINFO* >::iterator iter;
    CAPCONNECTINFO* pcapconnectinfo;
    QString strStatic,strTemp1,strTemp2,strStatic1;
    CWSSysconfig *wsSysconfig = CWSSysconfig::getInstance();
    SNIFFER_APP* pIED = NULL;
    double fKeep;
    //填充数据至table
    ui->tableWidget->setRowCount(pcappackagesmnger->m_mapcapconnectionfo.size());
    int nlinksize = 0;
    for (iter = pcappackagesmnger->m_mapcapconnectionfo.begin(); iter != pcappackagesmnger->m_mapcapconnectionfo.end(); iter ++ )
    {
        pcapconnectinfo = iter->second;
        if(pcapconnectinfo == NULL)
            continue;
        if(pcapconnectinfo->map_capmsginfo.size() == 0)//无数据
            continue;
        nlinksize ++;
        QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(nlinksize));
        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem((nlinksize-1),0,item1);
        if(pcapconnectinfo->ncapp_id >= 0)
        {
            pIED = wsSysconfig->I_GET_IED_BYAPPID(pcapconnectinfo->ncapp_id);
            if(pIED != NULL)
            {
                strTemp1.sprintf("%s",QString::fromLocal8Bit(pIED->cdesc).toStdString().c_str());
            }
        }
        if(pIED == NULL)//配置中没有
        {
            if(strlen(pcapconnectinfo->csrc1_ip) > 0)
            {
                strTemp1.sprintf("%s",pcapconnectinfo->csrc1_ip);
            }
            else
            {
                strTemp1.sprintf("%s",pcapconnectinfo->csrc1_mac);
            }
            pIED = wsSysconfig->I_GET_IED_BYNETIDADDR((char*)strTemp1.toStdString().c_str());//根据IP地址或去
            //strTemp1.ReleaseBuffer();
            if(pIED != NULL)
            {
                strTemp1.sprintf("%s",QString::fromLocal8Bit(pIED->cdesc).toStdString().c_str());
            }
        }
        if(strlen(pcapconnectinfo->csrc2_ip) > 0)
        {
            strTemp2.sprintf("%s",pcapconnectinfo->csrc2_ip);
        }
        else
        {
            strTemp2.sprintf("%s",pcapconnectinfo->csrc2_mac);
        }
        pIED = wsSysconfig->I_GET_IED_BYNETIDADDR((char*)strTemp2.toStdString().c_str());
        //strTemp2.ReleaseBuffer();
        if(pIED != NULL)
        {
            strTemp2.sprintf("%s",QString::fromLocal8Bit(pIED->cdesc).toStdString().c_str());
        }
        if(pcapconnectinfo->ncapp_id >= 0)
        {
            if(pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_SMV92)
            {
                strStatic.sprintf("连接%d SMV 0x%x",pcapconnectinfo->ncapp_id);
            }
            else if(pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_GOOSE)
            {
                strStatic.sprintf("GOOSE 0x%x",pcapconnectinfo->ncapp_id);
            }
            else
            {
                strStatic.sprintf("连接%d",pcapconnectinfo->nseq);
            }
        }
        else
        {
            if(pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_MMS)
            {
                strStatic.sprintf("连接%d MMS",pcapconnectinfo->nseq);
            }
            else
            {
                strStatic.sprintf("连接%d",pcapconnectinfo->nseq);
            }
        }
        if(pcapconnectinfo->nconnectapptype == PROTOAPPTYPE_SMV92)
        {
            strStatic1.sprintf("%s",strTemp1.toStdString().c_str());
        }
        else
        {
            strStatic1.sprintf("%s<-->%s",strTemp1.toStdString().c_str(),strTemp2.toStdString().c_str());
        }
        //连接名称
        QTableWidgetItem *item2 = new QTableWidgetItem(strStatic1);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem((nlinksize-1),1,item2);
        //总流量(字节)
        strTempValue.sprintf("%u",pcapconnectinfo->lTotalCapLenth);
        QTableWidgetItem *item3 = new QTableWidgetItem(strTempValue);
        item3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem((nlinksize-1),2,item3);
        //总帧数"
        if(pcapconnectinfo->nconnectapptype != PROTOAPPTYPE_SMV92 && pcapconnectinfo->nconnectapptype != PROTOAPPTYPE_GOOSE )
        {
            strTempValue.sprintf("%d(%d<->%d)",pcapconnectinfo->map_capmsginfo.size(),pcapconnectinfo->nsrc1packages,pcapconnectinfo->nsrc2packages);
        }
        else
        {
            strTempValue.sprintf("%d",pcapconnectinfo->map_capmsginfo.size());
        }
        QTableWidgetItem *item4 = new QTableWidgetItem(strTempValue);
        item4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem((nlinksize-1),3,item4);
        //异常报文帧数
        if (pcapconnectinfo->nconnectapptype == 3 || pcapconnectinfo->nconnectapptype == 5)
        {
            strTempValue.sprintf("%d",pcapconnectinfo->map_capmsginfo_error.size());
        }
        else
            strTempValue.sprintf("%d",pcapconnectinfo->map_capmsginfo_error.size()+pcapconnectinfo->map_capmsginfo_event.size());
        QTableWidgetItem *item5 = new QTableWidgetItem(strTempValue);
        item5->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem((nlinksize-1),4,item5);
        //帧速(帧/秒) 流量(Mb/秒
        fKeep = pcapconnectinfo->ftime_relative_last - pcapconnectinfo->ftime_relative_first;
        int nLiuLiang = 0;
        double fLiuLiang = 0.0f;
        if(fKeep > 0.0000001)
        {
            nLiuLiang = (int) pcapconnectinfo->map_capmsginfo.size()/fKeep;
            fLiuLiang = (double)pcapconnectinfo->lTotalCapLenth*8/(fKeep*1024*1024);
        }
        strTempValue.sprintf("%d",nLiuLiang);
        QTableWidgetItem *item6 = new QTableWidgetItem(strTempValue);
        item6->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem((nlinksize-1),5,item6);
        strTempValue.sprintf("%.3f",fLiuLiang);
        QTableWidgetItem *item7 = new QTableWidgetItem(strTempValue);
        item7->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem((nlinksize-1),6,item7);
    }
    ui->labelTotalLink->setText(QString::number(nlinksize));
    ui->tableWidget->resizeColumnsToContents();//根据内容自动调整所有列的列宽
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//设置表格最后列自适应
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置表格选择一整行
    //ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);//设置表格可多选
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);//设置表头不被选中高亮
    QFileInfo fi = QFileInfo(dir);
    this->setWindowTitle(("流量统计报告："+fi.fileName()));
}

/**
 * @brief StatisticalReport::saveFileToXls 保存报表内容至EXCEL
 */
void StatisticalReport::saveFileToXls()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("保存报表"),"",tr("EXCEL(*.xls)"));
    if(!fileName.isNull())
    {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this,"提示","保存文件失败！");
        }
        else
        {
            QTextStream streams(&file);
            QString head(tr("序号\t连接名称\t总流量（字节）\t总帧数\t异常报文帧数\t帧数（帧/秒）\t流量（Mb/秒）\n"));
            streams << head;
            for(int i=0;i<ui->tableWidget->rowCount();i++)
                for(int j=0;j<ui->tableWidget->columnCount();j++)
                {
                    QString qs = ui->tableWidget->item(i,j)->text()+"\t";
                    if(j==(ui->tableWidget->columnCount()-1))
                        qs = qs + "\n";
                    streams << qs;
                }
            file.flush();
            file.close();
        }
    }
}

