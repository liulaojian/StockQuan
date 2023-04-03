#include "dlgselectdate.h"
#include "ui_dlgselectdate.h"
#include "StockDataMgr.h"

DlgSelectDate::DlgSelectDate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSelectDate)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->setupUi(this);


    StockData* pStockData=stockDataMgr()->FindStockData("SH000001");
    QSharedPointer<StockDataInfo> pLastStockDataInfo=pStockData->GetLastStockDataInfo(STOCK_DATA_TYPE_DAY);
    QSharedPointer<StockDataInfo> pFirstStockDataInfo=pStockData->GetFirstStockDataInfo(STOCK_DATA_TYPE_DAY);

    QString strLastDateTime=pLastStockDataInfo->GetDate();
    strLastDateTime+=" 15:00";
    QDateTime mLastDateTime=QDateTime::fromString( strLastDateTime,"yyyy/MM/dd hh:mm");
    QDate mLastDate=mLastDateTime.date();


    QString strFirstDateTime=pFirstStockDataInfo->GetDate();
    strFirstDateTime+=" 15:00";
    QDateTime mFirstDateTime=QDateTime::fromString( strFirstDateTime,"yyyy/MM/dd hh:mm");
    QDate mFirstDate=mFirstDateTime.date();

    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit->setMinimumDate(mFirstDate);
    ui->dateEdit->setMaximumDate(mLastDate);
    ui->dateEdit->setDate(mLastDate);

}

DlgSelectDate::~DlgSelectDate()
{
    delete ui;
}

void DlgSelectDate::on_pushButton_Ok_clicked()
{
    mSelectDate=ui->dateEdit->date();
    accept();
}


void DlgSelectDate::on_pushButton_Cancel_clicked()
{
    reject();
}

