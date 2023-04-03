#include "dlgfunwait.h"
#include "ui_dlgfunwait.h"


DlgFunWait::DlgFunWait(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgFunWait)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
}

DlgFunWait::~DlgFunWait()
{
    delete ui;
}


void DlgFunWait::solt_fun_process(int percent)
{
    ui->progressBar->setValue(percent);
    if(percent>=100)
        reject();
}


void DlgFunWait::closeEvent(QCloseEvent *event)
{
    if(pFunGlobal)
    {
        pFunGlobal->StopFun();
        pFunGlobal->wait();
    }
}
