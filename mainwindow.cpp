#include <QtWidgets>
#include <QtGui>
#include "qmenu.h"
#include "qmenubar.h"

#include "mainwindow.h"
#include "finddialog.h"
#include "sortdialog.h"
#include "gotocell.h"
#include "spreadsheet.h"

QStringList MainWindow::recentFiles;
QStringList MainWindow::openFileList;

MainWindow::MainWindow()
{
    spreadsheet = new Spreadsheet;
    setCentralWidget(spreadsheet);

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

    setAttribute(Qt::WA_DeleteOnClose);

    readSettings();

    findDialog = 0;

    setWindowIcon(QIcon(":/images/icon.png"));
    setCurrentFile("");
}

void MainWindow::createActions(){
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new spreadsheet file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/images/save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save your work"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save As"), this);
    saveAsAction->setIcon(QIcon(":/images/saveas.png"));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save new work"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    closeAction = new QAction(tr("&Close"), this);
    closeAction->setShortcut(QKeySequence::Close);
    closeAction->setStatusTip(tr("Close this window"));
    connect(closeAction, &QAction::triggered, this, &QMainWindow::close);

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit application"));
    connect(exitAction, &QAction::triggered, qApp, &QApplication::closeAllWindows);

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    deleteAction = new QAction(tr("Delete"), this);
    deleteAction->setIcon(QIcon(":/images/delete.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("Delete"));
    connect(deleteAction, SIGNAL(triggered()), spreadsheet, SLOT(del()));

    cutAction = new QAction(tr("Cut"), this);
    cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Cut"));
    connect(cutAction, SIGNAL(triggered()), spreadsheet, SLOT(cut()));

    copyAction = new QAction(tr("Copy"), this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy"));
    connect(copyAction, SIGNAL(triggered()), spreadsheet, SLOT(copy()));

    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste"));
    connect(pasteAction, SIGNAL(triggered()), spreadsheet, SLOT(paste()));

    selectRowAction = new QAction(tr("&Row"), this);
    selectRowAction->setStatusTip(tr("Select current row cells in the spreadsheet"));
    connect(selectRowAction, SIGNAL(triggered()), spreadsheet, SLOT(selectCurrentRow()));

    selectColumnAction = new QAction(tr("&Column"), this);
    selectColumnAction->setStatusTip(tr("Select current column cells in the spreadsheet"));
    connect(selectColumnAction, SIGNAL(triggered()), spreadsheet, SLOT(selectCurrentColumn()));

    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setIcon(QIcon(":/images/open.png"));
    openAction->setStatusTip(tr("Open a file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    selectAllAction = new QAction(tr("&All"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("Select all the cells in the spreadsheet"));
    connect(selectAllAction, SIGNAL(triggered()), spreadsheet, SLOT(selectAll()));

    showGridAction = new QAction(tr("Show Grid"), this);
    showGridAction->setCheckable(true);
    showGridAction->setChecked(spreadsheet->showGrid());
    showGridAction->setStatusTip(tr("Show or hide the spreadsheet's grid"));
    connect(showGridAction, SIGNAL(toggled(bool)), spreadsheet, SLOT(setShowGrid(bool)));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    goToCellAction = new QAction(tr("Go to Cell"), this);
    goToCellAction->setIcon(QIcon(":/images/goto.png"));
    goToCellAction->setStatusTip(tr("Go to a cell by location"));
    connect(goToCellAction, &QAction::triggered, this, &MainWindow::goToCell);

    recalculateAction = new QAction(tr("Recalculate"), this);
    recalculateAction->setStatusTip(tr("Recalculate spreadsheet"));
    connect(recalculateAction, &QAction::triggered, spreadsheet, &Spreadsheet::recalculate);

    sortAction = new QAction(tr("&Sort"), this);
    sortAction->setStatusTip(tr("Sort the spreadsheet"));
    connect(sortAction, &QAction::triggered, this, &MainWindow::sort);

    autoRecalcAction = new QAction(tr("Autorecalculate"), this);
    autoRecalcAction->setCheckable(true);
    autoRecalcAction->setChecked(true);
    autoRecalcAction->setStatusTip(tr("Turn on autorecalcuation"));
    connect(autoRecalcAction, &QAction::toggled, spreadsheet, &Spreadsheet::setAutoRecalculate);

    findAction = new QAction(tr("&Find"), this);
    findAction->setStatusTip(tr("Search the document"));
    findAction->setIcon(QIcon(":/images/find.png"));
    connect(findAction, &QAction::triggered, this, &MainWindow::find);

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show information about the app"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    separatorAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);

    selectSubMenu = editMenu->addMenu(tr("&Select"));
    selectSubMenu->addAction(selectRowAction);
    selectSubMenu->addAction(selectColumnAction);
    selectSubMenu->addAction(selectAllAction);

    editMenu->addSeparator();
    editMenu->addAction(findAction);
    editMenu->addAction(goToCellAction);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(recalculateAction);
    toolsMenu->addAction(sortAction);

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(showGridAction);
    optionsMenu->addAction(autoRecalcAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::createContextMenu()
{
    spreadsheet->addAction(cutAction);
    spreadsheet->addAction(copyAction);
    spreadsheet->addAction(pasteAction);
    spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);
    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(findAction);
    editToolBar->addAction(goToCellAction);
}

void MainWindow::createStatusBar()
{
    locationLabel = new QLabel(" W999 ");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());
    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);
    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);
    connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)),
    this, SLOT(updateStatusBar()));
    connect(spreadsheet, SIGNAL(modified()),
    this, SLOT(spreadsheetModified()));
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    locationLabel->setText(spreadsheet->currentLocation());
    formulaLabel->setText(spreadsheet->currentFormula());
}

void MainWindow::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}

void MainWindow::newFile()
{
    MainWindow *mainWin = new MainWindow;
    mainWin->show();
}

bool MainWindow::okToContinue(){
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Spreadsheet"),
                                     tr("The document has been modified.\n"
                                        "Do you want to save your changes?"),
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return save();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::open()
{
    bool alreadyOpen;
    QString fileName;
    do {
        alreadyOpen = false;
        fileName = QFileDialog::getOpenFileName(this, tr("Open Spreadsheet"), ".",
                                                        tr("Spreadsheet files (*.sp)"));
        if (openFileList.contains(fileName)) {
            QMessageBox::critical(this, tr("Spreadsheet"), tr("File open in another window"));
            alreadyOpen = true;
        }
    } while (alreadyOpen == true);
    if (!fileName.isEmpty()) {
        MainWindow *m = new MainWindow;
        m->show();
        m->loadFile(fileName);
    }
    else
        QMessageBox::critical(this, tr("Spreadsheet"), tr("No file specified"));
}

bool MainWindow::loadFile(const QString &fileName)
{
    if (!spreadsheet->readFile(fileName)) {
        statusBar()->showMessage(tr("Loading canceled"), 2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveFile(const QString &fileName)
{
    if (!spreadsheet->writeFile(fileName)) {
        statusBar()->showMessage(tr("Saving canceled"), 2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Spreadsheet"), ".",
                                                    tr("Spreadsheet files (*.sp)"));
    if (fileName.isEmpty())
        return false;
    return saveFile(fileName);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        writeSettings();
        openFileList.removeAll(curFile);
        if (curFile.isEmpty())
                openFileList.removeAll(newName);
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);
    QString shownName = tr("Untitled");

    if (!curFile.isEmpty()) {
        //fileName is currently full path, we only want to display user readable name
        shownName = strippedName(curFile);
        openFileList.append(curFile);
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        newName = "";
        updateRecentFileActions();
    } else {
        //If multiple untitled (new) files are opened
        for (int i = 0;; ++i) {
            if (i != 0){
                shownName.append(QString::number(i));
            }
            //this will never run with existing files due to check in open()
            if (openFileList.contains(shownName))
                shownName.remove(QString::number(i));
            else {
                openFileList.append(shownName);
                newName = shownName;
                break;
            }

        }
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Spreadsheet")));
}

//Remove path
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::updateRecentFileActions()
{
    foreach (QWidget *win, QApplication::topLevelWidgets()){
        if (MainWindow *mainWin = qobject_cast<MainWindow *>(win))
            mainWin->updateRecentFileActionsAll();
    }
}

void MainWindow::updateRecentFileActionsAll()
{

    QMutableStringListIterator i(recentFiles);
    while (i.hasNext()) {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    for (int j = 0; j < MaxRecentFiles; ++j) {
        if (j < recentFiles.count()) {
            QString text = tr("&%1 %2").arg(j+1).arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        } else {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(recentFiles.isEmpty());
}

void MainWindow::openRecentFile()
{
    if (okToContinue()) {
        QAction *action = qobject_cast<QAction*>(sender());
        if (action)
            loadFile(action->data().toString());
    }
}

void MainWindow::find()
{
    if (!findDialog) {
        findDialog = new FindDialog(this);
        connect(findDialog, &FindDialog::findNext, spreadsheet, &Spreadsheet::findNext);
        connect(findDialog, &FindDialog::findPrev, spreadsheet, &Spreadsheet::findPrevious);
    }
    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}

void MainWindow::goToCell()
{
    Gotocell dialog(this);
    if (dialog.exec()) {
        QString str = dialog.lineEdit->text().toUpper();
        //QTableWidget starts at (0,0), so subtract 1 and 'A' ascii value to find location
        spreadsheet->setCurrentCell(str.mid(1).toInt() - 1, str[0].unicode() - 'A');
    }
}

void MainWindow::sort()
{
    Sortdialog dialog (this);
    QTableWidgetSelectionRange range = spreadsheet->selectedRange();
    dialog.setColumnRange('A' + range.leftColumn(), 'A' + range.rightColumn());
    if (dialog.exec()) {
        SpreadsheetCompare compare;
        compare.keys[0] = dialog.PrimarycomboBox->currentIndex();
        compare.keys[1] = dialog.SecondarycomboBox->currentIndex() - 1;
        compare.keys[2] = dialog.TertiarycomboBox->currentIndex() - 1;
        compare.ascending[0] = (dialog.PrimaryorderBox->currentIndex() == 0);
        compare.ascending[1] = (dialog.PrimaryorderBox->currentIndex() == 0);
        compare.ascending[2] = (dialog.PrimaryorderBox->currentIndex() == 0);
        spreadsheet->sort(compare);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Spreadsheet"), tr("Cali or bust"));
}

void MainWindow::writeSettings()
{
    QSettings settings("By Matthew May", "Spreadsheet");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("showGrid", showGridAction->isChecked());
    settings.setValue("autoRecalc", autoRecalcAction->isChecked());
}

void MainWindow::readSettings()
{
    QSettings settings("By Matthew May");

    //
    // If no default value saved (on startup), restore geometry will return false
    // and Qt will automatically set geometry.
    //
    restoreGeometry(settings.value("geometry").toByteArray());

    // If no settings file (startup), toStringList will return an empty list and recentFiles will be empty.
    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();

    bool showGrid = settings.value("showGrid", true).toBool();
    showGridAction->setChecked(showGrid);

    bool autoRecalc = settings.value("autoRecalc", true).toBool();
    autoRecalcAction->setChecked(autoRecalc);
}

