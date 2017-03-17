#ifndef DLGOPENMULTILIBCAPFILES_H
#define DLGOPENMULTILIBCAPFILES_H

#include <QDialog>
#include "LibpCapFileMnger.h"
namespace Ui {
class dlgOpenMultiLibCapfiles;
}

class dlgOpenMultiLibCapfiles : public QDialog
{
    Q_OBJECT

public:
    explicit dlgOpenMultiLibCapfiles(QWidget *parent = 0);
    ~dlgOpenMultiLibCapfiles();
    MAP_TS_LIBCAP_FILE* m_pmap_tslibcapfilelist;//自动按时间从老到新排序使用
    MAP_TS_LIBCAP_FILE saveFileInfo;
    void init();

private slots:
    void on_pushButtonLoadFile_clicked(bool checked);
    void btnAccepted();

    void on_pushButtonDel_clicked(bool checked);

    void on_pushButtonClear_clicked(bool checked);

    void on_pushButtonAutoRange_clicked(bool checked);

    void on_pushButtonMoveDown_clicked(bool checked);

    void on_pushButtonMoveUp_clicked(bool checked);

private:
    Ui::dlgOpenMultiLibCapfiles *ui;
    int fileNumber;//文件存入列表位置
    int fileLength;//文件个数
    void fillTable(MAP_TS_LIBCAP_FILE map_ts_libcap_file);
    void swapItem(int nu);
};

#endif // DLGOPENMULTILIBCAPFILES_H
