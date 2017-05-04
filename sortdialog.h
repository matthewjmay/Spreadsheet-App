#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>
#include "ui_sortdialog.h"

class Sortdialog : public QDialog, public Ui::SortDialog
{
    Q_OBJECT
public:
    Sortdialog(QWidget *parent =0);
    void setColumnRange(QChar first, QChar last);
};

#endif // SORTDIALOG_H
