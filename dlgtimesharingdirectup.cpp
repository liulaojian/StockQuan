#include "dlgtimesharingdirectup.h"
#include "ui_dlgtimesharingdirectup.h"

DlgTimeSharingDirectUp::DlgTimeSharingDirectUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgTimeSharingDirectUp)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
}

DlgTimeSharingDirectUp::~DlgTimeSharingDirectUp()
{
    delete ui;
}

void DlgTimeSharingDirectUp::on_pushButton_Ok_clicked()
{

     fUpValue=ui->spinBox_Up->value();

     fDownValue=ui->spinBox_Down->value();

     accept();
}


void DlgTimeSharingDirectUp::on_pushButton_Cancel_clicked()
{
    reject();
}

