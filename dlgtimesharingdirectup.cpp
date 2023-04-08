#include "dlgtimesharingdirectup.h"
#include "ui_dlgtimesharingdirectup.h"
#include <QSettings>

DlgTimeSharingDirectUp::DlgTimeSharingDirectUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgTimeSharingDirectUp)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->setupUi(this);

    QString inifile = QCoreApplication::applicationDirPath();
    inifile+="stockquan.ini";

    QSettings settings(inifile,QSettings::IniFormat);
    int upvalue = settings.value("timedirectup/upvalue").toInt();
    int downvalue = settings.value("timedirectup/downvalue").toInt();
    if(upvalue==0)
        upvalue=3;
    if(downvalue==0)
        downvalue=3;

    ui->spinBox_Up->setValue(upvalue);
    ui->spinBox_Down->setValue(downvalue);
}

DlgTimeSharingDirectUp::~DlgTimeSharingDirectUp()
{
    delete ui;
}

void DlgTimeSharingDirectUp::on_pushButton_Ok_clicked()
{

     fUpValue=ui->spinBox_Up->value();

     fDownValue=ui->spinBox_Down->value();

     QString inifile = QCoreApplication::applicationDirPath();
     inifile+="stockquan.ini";

     QSettings settings(inifile,QSettings::IniFormat);
     settings.setValue("timedirectup/upvalue",ui->spinBox_Up->value());
     settings.setValue("timedirectup/downvalue",ui->spinBox_Down->value());


     accept();
}


void DlgTimeSharingDirectUp::on_pushButton_Cancel_clicked()
{
    reject();
}

