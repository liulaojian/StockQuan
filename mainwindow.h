#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool InitStockList(void);

private:
    QToolButton* CreateToolButton(QString text, QString icon);

private slots:
    void btn_stocklist_click();
    void btn_jetteonfilter_click();
    void solt_stocklist_customContextMenuRequested(const QPoint &pos);
    void solt_stockActionOpen_triggered(QAction *action);
private:
    Ui::MainWindow *ui;

    QMenu *m_pStockListMenu;
    QAction* m_pActionOpen;

    // QWidget interface
protected:
    void changeEvent(QEvent *);
};
#endif // MAINWINDOW_H
