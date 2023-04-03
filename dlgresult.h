#ifndef DLGRESULT_H
#define DLGRESULT_H

#include <QDialog>
#include <QVector>

namespace Ui {
class DlgResult;
}

class DlgResult : public QDialog
{
    Q_OBJECT

public:
    explicit DlgResult(QWidget *parent = nullptr);
    ~DlgResult();

    void InitList(void);

    void SetResultList(QVector<QString> list) {vecResultCodeList=list;}

private:
    QVector<QString> vecResultCodeList;
private:
    Ui::DlgResult *ui;
};

#endif // DLGRESULT_H
