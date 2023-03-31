#pragma execution_character_set("utf-8")
#include "dlgstocklist.h"
#include "ui_dlgstocklist.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QDebug>
#include <QMessageBox>
DlgStockList::DlgStockList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgStockList)
{
    ui->setupUi(this);
    ui->listWidget->setIconSize(QSize(50,50));
    QListWidgetItem *add_item=new QListWidgetItem(ui->listWidget);
    add_item->setIcon(QIcon(":/image/list.png"));
    add_item->setText("Add");
    add_item->setTextAlignment(Qt::AlignVCenter);
    //ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    QListWidgetItem* cuby_item=new QListWidgetItem(QIcon(":/image/basic.png"),"cuby",ui->listWidget);

    QListWidgetItem *dropbox_item = new QListWidgetItem();
    dropbox_item->setIcon(QIcon(":/image/extend.png"));
    dropbox_item->setText(tr("Dropbox"));
    ui->listWidget->addItem(dropbox_item);
    ui->listWidget->setCurrentRow(1);


    QWidget *widget = new QWidget(ui->listWidget);
    QListWidgetItem *widget_item = new QListWidgetItem();
    widget_item->setIcon(QIcon(":/image/extend.png"));
    ui->listWidget->addItem(widget_item);
    //实现替换，自定义item
    QVBoxLayout *pNewLayOut=new QVBoxLayout();

    QComboBox *comboBox=new QComboBox(widget);
    comboBox->addItem("first",1);
    pNewLayOut->addWidget(comboBox);
    widget->setLayout(pNewLayOut);



    ui->listWidget->setItemWidget(widget_item,widget);

    ui->listWidget->setStyleSheet("QListWidget{border:1px solid gray; color:black;outline:0px;}"
                               "QListWidget::Item{padding-top:20px; padding-bottom:4px; }"
                               "QListWidget::Item:hover{background:skyblue; }"
                               "QListWidget::item:selected{background:lightgray; color:red; }"
                               "QListWidget::item:selected:!active{border-width:0px; background:lightgreen; }"
                               );




    ui->tableWidget->setRowCount(15);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->verticalHeader()->setVisible(false);
    QStringList header;
    header<<"代码"<<"名称"<<"行业"<<"现价"<<"流通市值";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;color: black;}");
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidget->setItem(0,0,new QTableWidgetItem("SH60001"));
    ui->tableWidget->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(0,0)->setData(Qt::UserRole,0);

    connect(ui->tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(solt_currentCellChanged(int,int,int,int)));

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_pContextMenu=new QMenu(this);
    m_pAction=new QAction(this);
    m_pAction->setText("测试");
    m_pContextMenu->addAction(m_pAction);

    m_pContextMenu->addSeparator();
    m_pContextMenu->addAction(QIcon(":/image/basic.png"),"50万");

    m_pContextMenu->addAction("100万");




    m_pContextMenu->setStyleSheet("color: rgb(220, 2, 2);");

    m_pContextMenu->setStyleSheet("QMenu{background-color: white;border: 1px solid white;}\
            QMenu::item {padding:2px 2px 2px 2px;border-bottom:0px solid #DBDBDB;margin:0px 0px 0px 0px;}\
            QMenu::item:selected {background-color: rgb(255,0,0);}");
    connect(ui->tableWidget,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(solt_customContextMenuRequested(const QPoint&)));
    connect(m_pContextMenu,SIGNAL(triggered(QAction *)),this,SLOT(solt_triggered(QAction *)));
}

DlgStockList::~DlgStockList()
{
    delete ui;
}


void DlgStockList::solt_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(previousRow!=currentRow)
    {
        qDebug()<<"previousRow="<<previousRow<<"   currentRow="<<currentRow<<"\n";
        QTableWidgetItem *item=ui->tableWidget->item(currentRow,0);
        if(item)
        {
            QVariant  vardata=item->data(Qt::UserRole);
            int userdata=-1;
            if(!vardata.isNull())
                userdata= vardata.toInt();
            qDebug()<<"userdata="<<userdata<<"\n";
        }

    }
}

void DlgStockList::solt_customContextMenuRequested(const QPoint &pos)
{
    qDebug()<<"solt_customContextMenuRequested"<<"\n";
   /* pMenuSelectItem = ui->tableWidget->itemAt(pos);
    if(pMenuSelectItem != nullptr){

    }*/

     m_pContextMenu->exec(QCursor::pos());
}

void DlgStockList::solt_triggered(QAction *action)
{
  if(action==m_pAction)
  {
    QMessageBox::information(this,"注意","solt_triggered");
  }
}
