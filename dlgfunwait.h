#ifndef DLGFUNWAIT_H
#define DLGFUNWAIT_H

#include <QDialog>
#include "FunGlobal.h"
namespace Ui {
class DlgFunWait;
}

class DlgFunWait : public QDialog
{
    Q_OBJECT

public:
    explicit DlgFunWait(QWidget *parent = nullptr);
    ~DlgFunWait();

    void SetFunGlobal(FunGlobal *p) {pFunGlobal=p;}
private:
    FunGlobal *pFunGlobal=nullptr;
public slots:
    void solt_fun_process(int percent);
private:
    Ui::DlgFunWait *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // DLGFUNWAIT_H
