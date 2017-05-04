#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QCheckBox;
class QPushButton;
class QLineEdit;
class QLabel;

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = 0);

private slots:
    void findPressed();
    void enableFindButton(const QString &str);

signals:
    void findNext(const QString &string, Qt::CaseSensitivity cs);
    void findPrev(const QString &string, Qt::CaseSensitivity cs);

private:
    QCheckBox *caseCheckbox;
    QCheckBox *searchBackwards;
    QLineEdit *findWord;
    QPushButton *search;
    QPushButton *close;
    QLabel *label;

};

#endif // FINDDIALOG_H
