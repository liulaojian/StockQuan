#ifndef DLGSELECTDATE_H
#define DLGSELECTDATE_H

#include <QDialog>
#include <QDate>
namespace Ui {
class DlgSelectDate;
}

class DlgSelectDate : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSelectDate(QWidget *parent = nullptr);
    ~DlgSelectDate();

    QDate GetSelectDate(void) {return mSelectDate;}
private slots:
    void on_pushButton_Ok_clicked();

    void on_pushButton_Cancel_clicked();

private:
    QDate mSelectDate;
private:
    Ui::DlgSelectDate *ui;
};

#endif // DLGSELECTDATE_H
