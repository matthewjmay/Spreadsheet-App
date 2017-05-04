#include "sortdialog.h"
#include <QWidget>

Sortdialog::Sortdialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    SecondarygroupBox->hide();
    TertiarygroupBox->hide();

    layout()->setSizeConstraint(QLayout::SetFixedSize);

    setColumnRange('A', 'Z');
}

void Sortdialog::setColumnRange(QChar first, QChar last){
    PrimarycomboBox->clear();
    SecondarycomboBox->clear();
    TertiarycomboBox->clear();

    SecondarycomboBox->addItem(tr("None"));
    TertiarycomboBox->addItem(tr("None"));
    PrimarycomboBox->setMinimumSize(SecondarycomboBox->sizeHint());

    QChar ch = first;
    while (ch <=last){
        PrimarycomboBox->addItem(QString(ch));
        SecondarycomboBox->addItem(QString(ch));
        TertiarycomboBox->addItem(QString(ch));
        ch = ch.unicode() + 1;
    }
}
