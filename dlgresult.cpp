#pragma execution_character_set("utf-8")
#include "dlgresult.h"
#include "ui_dlgresult.h"
#include "StockDataMgr.h"
DlgResult::DlgResult(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgResult)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->setupUi(this);

    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->verticalHeader()->setVisible(false);

    QStringList header;
    header<<"序号"<<"代码"<<"名称";
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;color: black;}");
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget->setColumnWidth(1,150);
    ui->tableWidget->setColumnWidth(2,150);


}

DlgResult::~DlgResult()
{
    delete ui;
}

void DlgResult::InitList(void)
{
    ui->tableWidget->setRowCount(vecResultCodeList.size());
    StockData* pStockData=nullptr;
    for(int i=0;i<vecResultCodeList.size();i++)
    {
        pStockData=stockDataMgr()->FindStockData(vecResultCodeList[i]);
        if(pStockData)
        {
            QString strId=QString::asprintf("%d",i);
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(strId));
            ui->tableWidget->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            ui->tableWidget->setItem(i,1,new QTableWidgetItem(pStockData->GetStockCode()));
            ui->tableWidget->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            QVariant data=QVariant::fromValue((void*)pStockData);
            ui->tableWidget->item(i,1)->setData(Qt::UserRole,data);

            ui->tableWidget->setItem(i,2,new QTableWidgetItem(pStockData->GetStockName()));
            ui->tableWidget->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
}
