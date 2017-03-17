#ifndef TIMESTYLESETUP_H
#define TIMESTYLESETUP_H

#include <QDialog>

namespace Ui {
class TimeStyleSetup;
}

class TimeStyleSetup : public QDialog
{
    Q_OBJECT

public:
    explicit TimeStyleSetup(QWidget *parent = 0);
    ~TimeStyleSetup();

private:
    Ui::TimeStyleSetup *ui;
private slots:
    void onRadioClicked();
    void btnAccepted();
public:
    int m_nradTimeType;
};

#endif // TIMESTYLESETUP_H
