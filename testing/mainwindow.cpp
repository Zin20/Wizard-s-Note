#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
bool MainWindow::save()
{
    if(currentFileName.isEmpty())
    {
        return saveAs();
    }
    else
    {
        return saveFile(currentFileName);
    }
}



bool MainWindow::isSave()
{
    if(textEdit->document()->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("T Note"),tr("The document has been modified.\nDo you want to save your changes?"),
                                   QMessageBox::Save| QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save)
        {
           return save();
        }
        else if(ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}
void MainWindow::open()
{
    if(isSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        if(!fileName.isEmpty())
        {
            loadFile(fileName);
        }
    }
}

void MainWindow::newFile()
{
   if(isSave())
   {
       textEdit->clear();
       setCurrentFile("");
   }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if(fileName.isEmpty())
    {
        return false;
    }
    return saveFile(fileName);
}
void MainWindow::copy()
{

}

void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFileName = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString showName = currentFileName;
    if(currentFileName.isEmpty())
     {
        showName = "untitled.txt";
    }
    setWindowFilePath(showName);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,tr("T Note"),tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);

    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif
        out << textEdit->toPlainText();
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"),2000);
    return true;
}
void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    QFileInfo tempFile(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,tr("T Note"),tr("Cannot read file %1:\n%2").arg(fileName).arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
    #ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif
     textEdit->setPlainText(in.readAll());
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
     #endif
     setCurrentFile(fileName);

     *curDir = tempFile.dir();
     populateList();
     statusBar()->showMessage(tr("File loaded"),2000);

}

void MainWindow::setDocks()
{
    fileDock = new QDockWidget("",this);
    fileList = new QListWidget;
    fileDock->setWidget(fileList);
    fileDock->setAllowedAreas(Qt::LeftDockWidgetArea);
    fileDock->setWindowFlags(Qt::CustomizeWindowHint);

}
void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"),this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new File"));
    connect(newAct,SIGNAL(triggered()),this,SLOT(newFile()));

    openAct = new QAction(tr("&Open"),this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Opens a new file"));
    connect(openAct,SIGNAL(triggered()),this,SLOT(open()));


    savAct = new QAction(tr("&Save"),this);
    savAct->setShortcuts(QKeySequence::Save);
    savAct->setStatusTip(tr("Saves the current file"));
    connect(savAct,SIGNAL(triggered()),this,SLOT(save()));

    savAsAct = new QAction(tr("&Save As"),this);
    savAsAct->setShortcut(QKeySequence::SaveAs);
    savAsAct->setStatusTip(tr("Save the file as"));
    connect(savAsAct,SIGNAL(triggered()),this,SLOT(saveAs()));

    cutAct = new QAction(tr("&Cut"),this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Copy and deletes the highlighted text"));
    connect(cutAct,SIGNAL(triggered()),textEdit,SLOT(cut()));
    cutAct->setEnabled(false);
    connect(textEdit,SIGNAL(copyAvailable(bool)),cutAct,SLOT(setEnabled(bool)));

    copyAct = new QAction(tr("&Copy"),this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copys to clipboard"));
    connect(copyAct,SIGNAL(triggered()),textEdit,SLOT(copy()));// Needs to be textEdit so it can communcaite
    copyAct->setEnabled(false);

    connect(textEdit, SIGNAL(copyAvailable(bool)),copyAct,SLOT(setEnabled(bool)));

    pasteAct = new QAction(tr("&Paste"),this);
    pasteAct->setShortcuts(QKeySequence::Copy);
    pasteAct->setStatusTip(tr("Paste from clipboard"));
    connect(pasteAct,SIGNAL(triggered()),textEdit,SLOT(paste()));




}
void MainWindow::createMenus()
{
    fileTool = menuBar()->addMenu(tr("&File"));
    fileTool->addAction(newAct);
    fileTool->addAction(openAct);
    fileTool->addAction(savAct);
    fileTool->addAction(savAsAct);

    editTool = menuBar()->addMenu(tr("&Edit"));
    editTool->addAction(cutAct);
    editTool->addAction(copyAct);
    editTool->addAction(pasteAct);
}
void MainWindow::populateList()
{

    if(fileList->count() != 0)
    {
        fileList->clear();
    }
    QStringList files = curDir->entryList(QDir::Files);
    fileList->addItems(files);
    //fileList->addItem(curDir->currentPath());


}

MainWindow::MainWindow(QWidget *parent)
{
 textEdit = new QTextEdit;
 // layout->addWidget(textEdit);
 filter = new QStringList;
 (*filter) << "*.txt"; // Later on will use XML
 curDir = new QDir;

 setCentralWidget(textEdit);
 setDocks();
 addDockWidget(Qt::LeftDockWidgetArea,fileDock);
 setWindowTitle("T Note");
 setMinimumSize(800,600);
 connect(textEdit->document(), SIGNAL(contentsChanged()),this, SLOT(documentWasModified()));
 setCurrentFile("");

 createActions();
 createMenus();

 // setActions();
  //fileTool = addToolBar("File");
  //fileTool->addAction(QIcon(""),"File");
  //fileTool->addAction(QIcon(""),"Testing");

}



MainWindow::~MainWindow()
{

}

