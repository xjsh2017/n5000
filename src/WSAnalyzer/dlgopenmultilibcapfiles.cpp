#ifdef OS_WINDOWS
#pragma execution_character_set("UTF-8")
#endif
#include "dlgopenmultilibcapfiles.h"
#include "ui_dlgopenmultilibcapfiles.h"
#include "wssysconfig.h"
#include <QFileDialog>
#include <QDateTime>
dlgOpenMultiLibCapfiles::dlgOpenMultiLibCapfiles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgOpenMultiLibCapfiles)
{
    ui->setupUi(this);    
    fileNumber = 0;
    fileLength = 0;
    m_pmap_tslibcapfilelist = NULL;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    connect(ui->pushButtonLoadFile,SIGNAL(clicked(bool)),this, SLOT(on_pushButtonLoadFile_clicked()));
    connect(ui->pushButtonDel,SIGNAL(clicked(bool)),this, SLOT(on_pushButtonDel_clicked()));
    connect(ui->pushButtonClear,SIGNAL(clicked(bool)),this, SLOT(on_pushButtonClear_clicked()));
    connect(ui->pushButtonAutoRange,SIGNAL(clicked(bool)),this, SLOT(on_pushButtonAutoRange_clicked()));
    connect(ui->pushButtonMoveDown,SIGNAL(clicked(bool)),this, SLOT(on_pushButtonMoveDown_clicked()));
    connect(ui->pushButtonMoveUp,SIGNAL(clicked(bool)),this, SLOT(on_pushButtonMoveUp_clicked()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(btnAccepted()));
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView {background-color:lblue } QHeaderView::section {background-color:lightblue;color: black;padding-left: 4px;}");
    ui->tableWidget->setStyleSheet("QTableWidget::item:selected { background-color: rgb(255,192,203) }");
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

dlgOpenMultiLibCapfiles::~dlgOpenMultiLibCapfiles()
{
    delete ui;
}
/**
 * @brief dlgOpenMultiLibCapfiles::init 初始化
 */
void dlgOpenMultiLibCapfiles::init()
{
    if(m_pmap_tslibcapfilelist->size() == 0)
        return;
    fileLength = m_pmap_tslibcapfilelist->size();
    std::map<int, TS_LIBCAP_FILE*>::iterator iter;
    TS_LIBCAP_FILE* ptscapfile_src;
    for(iter = m_pmap_tslibcapfilelist->begin(); iter != m_pmap_tslibcapfilelist->end(); iter ++ )
    {
        ptscapfile_src = iter->second;
        saveFileInfo[ptscapfile_src->n_filecreatetime] = ptscapfile_src;
    }
    fillTable(*m_pmap_tslibcapfilelist);
}
/**
 * @brief dlgOpenMultiLibCapfiles::on_pushButtonLoadFile_clicked 选择文件
 * @param checked
 */
void dlgOpenMultiLibCapfiles::on_pushButtonLoadFile_clicked(bool checked)
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
            tr("选择多文件"),
            "",
            tr("PCAP Files (*.PCAP)"));
    if(!fileNames.isEmpty())
    {
        TS_LIBCAP_FILE * ptslibcapfile;
        MAP_TS_LIBCAP_FILE map_ts_libcap_file;
        fileLength = fileLength + fileNames.size();
        foreach(QString str, fileNames)
        {
            QFileInfo fi = QFileInfo(str);
            ptslibcapfile = new TS_LIBCAP_FILE;
            ptslibcapfile->n_filesize = fi.size();
            ptslibcapfile->strfilename = fi.fileName().toStdString();
            ptslibcapfile->strfilepathname = fi.absolutePath().toStdString()+"/";
            ptslibcapfile->n_filecreatetime = fi.lastModified().toTime_t();
            saveFileInfo[ptslibcapfile->n_filecreatetime] = ptslibcapfile;
            map_ts_libcap_file[ptslibcapfile->n_filecreatetime] = ptslibcapfile;
        }
        fillTable(map_ts_libcap_file);
    }
}
/**
 * @brief dlgOpenMultiLibCapfiles::fillTable 填充选择的文件列表
 * @param map_ts_libcap_file 文件信息
 */
void dlgOpenMultiLibCapfiles::fillTable(MAP_TS_LIBCAP_FILE map_ts_libcap_file)
{
    TS_LIBCAP_FILE * ptslibcapfileEX;
    std::map<int, TS_LIBCAP_FILE*>::iterator iter;
    ui->tableWidget->setRowCount(fileLength);
    for(iter = map_ts_libcap_file.begin(); iter != map_ts_libcap_file.end(); iter ++ )
    {
        ptslibcapfileEX = iter->second;
        QTableWidgetItem * itemName = new QTableWidgetItem(QString::fromStdString(ptslibcapfileEX->strfilename));
        itemName->setTextAlignment(Qt::AlignCenter);
        //itemName->setData(Qt::UserRole,QVariant::fromValue(ptslibcapfileEX->strfilename));
        ui->tableWidget->setItem(fileNumber,0,itemName);
        QString strTime;
        time_t ttime = (time_t)ptslibcapfileEX->n_filecreatetime;
        struct tm *local;
        local=localtime(&ttime);
        char buf[80];
        strftime(buf,80,"%Y-%m-%d %H:%M:%S",local);
        strTime = buf;
        QTableWidgetItem * itemTime = new QTableWidgetItem(strTime);
        itemTime->setTextAlignment(Qt::AlignCenter);
        itemTime->setData(Qt::UserRole,QVariant::fromValue(ptslibcapfileEX->n_filecreatetime));
        ui->tableWidget->setItem(fileNumber,1,itemTime);
        QTableWidgetItem * itemSize = new QTableWidgetItem(QString::number(ptslibcapfileEX->n_filesize/1024.0,'f',1));
        //itemSize->setData(Qt::UserRole,QVariant::fromValue(ptslibcapfileEX->n_filesize));
        itemSize->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(fileNumber,2,itemSize);
        fileNumber++;
    }
    ui->tableWidget->resizeColumnsToContents();//根据内容自动调整所有列的列宽
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//设置表格最后列自适应
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置表格选择一整行
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);//设置表格可多选
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);//设置表头不被选中高亮
}
/**
 * @brief dlgOpenMultiLibCapfiles::btnAccepted 确认
 */
void dlgOpenMultiLibCapfiles::btnAccepted()
{
    TS_LIBCAP_FILE * ptslibcapfileEX;
    m_pmap_tslibcapfilelist->clear();
    for(int i = 0;i < ui->tableWidget->rowCount();i++)
    {
        QTableWidgetItem *m_time = ui->tableWidget->item(i,1);
        int n_time = m_time->data(Qt::UserRole).value<int>();
        m_pmap_tslibcapfilelist->insert(std::map <int, TS_LIBCAP_FILE*> :: value_type(i, saveFileInfo[n_time]));
    }
}
/**
 * @brief dlgOpenMultiLibCapfiles::on_pushButtonDel_clicked 删除文件列表
 * @param checked
 */
void dlgOpenMultiLibCapfiles::on_pushButtonDel_clicked(bool checked)
{
    int rowIndex = ui->tableWidget->currentRow();
    if (rowIndex != -1)
    {
        QTableWidgetItem *m_time = ui->tableWidget->item(rowIndex,1);
        int n_time = m_time->data(Qt::UserRole).value<int>();
        saveFileInfo.erase(saveFileInfo.find(n_time));
        ui->tableWidget->removeRow(rowIndex);
    }
    fileNumber--;
    fileLength--;
}
/**
 * @brief dlgOpenMultiLibCapfiles::on_pushButtonClear_clicked 清空文件列表
 * @param checked
 */
void dlgOpenMultiLibCapfiles::on_pushButtonClear_clicked(bool checked)
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();
    saveFileInfo.clear();
    fileNumber = 0;
    fileLength = 0;
}
/**
 * @brief dlgOpenMultiLibCapfiles::on_pushButtonAutoRange_clicked 自动排序文件列表
 * @param checked
 */
void dlgOpenMultiLibCapfiles::on_pushButtonAutoRange_clicked(bool checked)
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();
    fileNumber = 0;
    fillTable(saveFileInfo);
}
/**
 * @brief dlgOpenMultiLibCapfiles::on_pushButtonMoveDown_clicked 向下移动文件列表
 * @param checked
 */
void dlgOpenMultiLibCapfiles::on_pushButtonMoveDown_clicked(bool checked)
{
    swapItem(1);
}
/**
 * @brief dlgOpenMultiLibCapfiles::on_pushButtonMoveUp_clicked 向上移动文件列表
 * @param checked
 */
void dlgOpenMultiLibCapfiles::on_pushButtonMoveUp_clicked(bool checked)
{
    swapItem(-1);
}
/**
 * @brief dlgOpenMultiLibCapfiles::swapItem 交换文件列表位置
 * @param nu 向上向下方向控制
 */
void dlgOpenMultiLibCapfiles::swapItem(int nu)
{
    int rowIndex = ui->tableWidget->currentRow();
    if (rowIndex != -1)
    {
        if(nu > 0)
        {
            if((rowIndex+1)==ui->tableWidget->rowCount())
                return;
        }
        else
        {
            if((rowIndex-1) < 0)
                return;
        }
        QTableWidgetItem *m_filename = ui->tableWidget->item(rowIndex,0);
        QString fileName = m_filename->text();
        QTableWidgetItem *m_time = ui->tableWidget->item(rowIndex,1);
        int n_time = m_time->data(Qt::UserRole).value<int>();
        QString timeInfo = m_time->text();
        QTableWidgetItem *m_size = ui->tableWidget->item(rowIndex,2);
        QString size = m_size->text();
        QTableWidgetItem *m_filename_next = ui->tableWidget->item((rowIndex+nu),0);
        QTableWidgetItem *m_time_next = ui->tableWidget->item((rowIndex+nu),1);
        QTableWidgetItem *m_size_next = ui->tableWidget->item((rowIndex+nu),2);
        m_filename->setText(m_filename_next->text());
        m_time->setText(m_time_next->text());
        m_size->setText(m_size_next->text());
        m_time->setData(Qt::UserRole,QVariant::fromValue(m_time_next->data(Qt::UserRole).value<int>()));
        m_filename_next->setText(fileName);
        m_time_next->setText(timeInfo);
        m_size_next->setText(size);
        m_time_next->setData(Qt::UserRole,QVariant::fromValue(n_time));
        ui->tableWidget->selectRow(rowIndex+nu);
    }
}
