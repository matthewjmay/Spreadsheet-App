#include <QWidget>
#include <QtGui>

#include "mainwindow.h"
#include "finddialog.h"
#include "sortdialog.h"
#include "spreadsheet.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    spreadsheet = new Spreadsheet;
    setCentralWidget(spreadsheet);
    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();
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

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    selectAllAction = new QAction(tr("&All"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("Select all the cells in the spreadsheet"));
    connect(selectAllAction, SIGNAL(triggered()), spreadsheet, SLOT(selectAll()));

    showGridAction = new QAction(tr("&Show Grid"), this);
    showGridAction->setCheckable(true);
    showGridAction->setChecked(spreadsheet->showGrid());
    showGridAction->setStatusTip(tr("Show or hide the spreadsheet's grid"));
    connect(showGridAction, SIGNAL(toggled(bool)), spreadsheet, SLOT(setShowGrid(bool)));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
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
 if (okToContinue()) {
 spreadsheet->clear();
 setCurrentFile("");
 }
}

bool MainWindow::okToContinue()
{
 if (isWindowModified()) {
 int r = QMessageBox::warning(this, tr("Spreadsheet"),
 tr("The document has been modified.\n"
 "Do you want to save your changes?"),
 QMessageBox::Yes | QMessageBox::No
 | QMessageBox::Cancel);
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
 if (okToContinue()) {
 QString fileName = QFileDialog::getOpenFileName(this,
 tr("Open Spreadsheet"), ".",
 tr("Spreadsheet files (*.sp)"));
 if (!fileName.isEmpty())
 loadFile(fileName);
 }
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
 QString fileName = QFileDialog::getSaveFileName(this,
 tr("Save Spreadsheet"), ".",
 tr("Spreadsheet files (*.sp)"));
 if (fileName.isEmpty())
 return false;
 return saveFile(fileName);
}