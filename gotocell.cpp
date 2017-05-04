#include "gotocell.h"
#include <QtWidgets>

Gotocell::Gotocell(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
    lineEdit-> setValidator(new QRegExpValidator(regExp, this));

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void Gotocell::on_lineEdit_textChanged(){
    okButton->setEnabled(lineEdit->hasAcceptableInput());
}
