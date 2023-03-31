#include "StockChart.h"
#include <QPainter>

StockChart::StockChart(QWidget *parent):QWidget(parent)
{

}

StockChart::~StockChart()
{

}

void StockChart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter    painter(this);

    QRect rect(0,0,width(),height()); //viewport矩形区
    painter.setViewport(rect);//设置Viewport
    painter.setWindow(0,0,120,50); // 设置窗口大小，逻辑坐标
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QColor mColorBorder(255,0,0);
     QColor mColorBack(255,255,255);
    QPen    pen;//设置画笔
    pen.setWidth(2); //线宽
    pen.setColor(mColorBorder); //划线颜色
    pen.setStyle(Qt::SolidLine);//线的类型，实线、虚线等
    pen.setCapStyle(Qt::FlatCap);//线端点样式
    pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
    painter.setPen(pen);

    QBrush  brush;//设置画刷
    brush.setColor(mColorBack); //画刷颜色
    brush.setStyle(Qt::SolidPattern); //画刷填充样式
    painter.setBrush(brush);


    QFontMetrics    textSize(this->font());
    QString powStr=QString::asprintf("%d%%",77);
    QRect textRect=textSize.boundingRect(powStr);//得到字符串的rect

    painter.setFont(this->font());
    pen.setColor(mColorBorder); //划线颜色
    painter.setPen(pen);

    painter.drawText(55-textRect.width()/2,
               23+textRect.height()/2,
               powStr);


}
