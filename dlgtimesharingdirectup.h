#ifndef DLGTIMESHARINGDIRECTUP_H
#define DLGTIMESHARINGDIRECTUP_H

#include <QDialog>

namespace Ui {
class DlgTimeSharingDirectUp;
}

class DlgTimeSharingDirectUp : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTimeSharingDirectUp(QWidget *parent = nullptr);
    ~DlgTimeSharingDirectUp();

    float GetUpValue(void)  {return fUpValue;}
    float GetDownValue(void) {return fDownValue;}
private:
    float fUpValue=3.0;
    float fDownValue=3.0;

private slots:
    void on_pushButton_Ok_clicked();

    void on_pushButton_Cancel_clicked();

private:
    Ui::DlgTimeSharingDirectUp *ui;
};

#endif // DLGTIMESHARINGDIRECTUP_H
