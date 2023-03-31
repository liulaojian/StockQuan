#ifndef DLGSTOCKLIST_H
#define DLGSTOCKLIST_H

#include <QDialog>
#include <QMenu>
#include <QTableWidgetItem>

namespace Ui {
class DlgStockList;
}

class DlgStockList : public QDialog
{
    Q_OBJECT

public:
    explicit DlgStockList(QWidget *parent = nullptr);
    ~DlgStockList();

private slots:
    void solt_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void solt_customContextMenuRequested(const QPoint &pos);
    void solt_triggered(QAction *action);
private:
    Ui::DlgStockList *ui;
    QMenu *m_pContextMenu;
    QAction* m_pAction;

    QTableWidgetItem *pMenuSelectItem=nullptr;
};

#endif // DLGSTOCKLIST_H
