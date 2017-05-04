#include "finddialog.h"
#include <QtWidgets>

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent)
{
    caseCheckbox = new QCheckBox(tr("Match &case?"));
    searchBackwards = new QCheckBox(tr("Search Backwards?"));
    findWord = new QLineEdit;
    search = new QPushButton("Search");
    close = new QPushButton("Close");
    label = new QLabel("Find:");
    label->setBuddy(findWord);

    connect(findWord, SIGNAL(textChanged(const QString&)), this, SLOT(enableFindButton(const QString&)));
    connect(close, SIGNAL(clicked()), this, SLOT(close()));
    connect(search, SIGNAL(clicked()), this, SLOT(findPressed()));

    search-> setEnabled(false);
    QHBoxLayout *topleftlayout = new QHBoxLayout;
    topleftlayout->addWidget(label);
    topleftlayout-> addWidget(findWord);

    QVBoxLayout *leftlayout = new QVBoxLayout;
    leftlayout-> addLayout(topleftlayout);
    leftlayout-> addWidget(caseCheckbox);
    leftlayout-> addWidget(searchBackwards);

    QVBoxLayout *rightlayout = new QVBoxLayout;
    rightlayout-> addWidget(search);
    rightlayout-> addWidget(close);

    QHBoxLayout *mainlayout = new QHBoxLayout;
    mainlayout-> addLayout(leftlayout);
    mainlayout-> addLayout(rightlayout);
    setLayout(mainlayout);

    setWindowTitle("Find");
    setFixedHeight(sizeHint().height());
}

void FindDialog::findPressed(){
    QString text = findWord->text();
    Qt::CaseSensitivity cs = caseCheckbox->isChecked() ? Qt::CaseSensitive
                                                       : Qt::CaseInsensitive;
    if(searchBackwards->isChecked())
        emit findPrev(text, cs);
    else
        emit findNext(text, cs);
}

void FindDialog::enableFindButton(const QString &str){
    search->setEnabled(!str.isEmpty());
}
