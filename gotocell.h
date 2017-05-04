#ifndef GOTOCELL_H
#define GOTOCELL_H

#include <QDialog>
#include <ui_gotocell.h>

class Gotocell : public QDialog, public Ui::GoToCellDialogue
{
    Q_OBJECT

public:
    explicit Gotocell(QWidget *parent = 0);

private slots:
    void on_lineEdit_textChanged();
};

#endif // GOTOCELL_H
