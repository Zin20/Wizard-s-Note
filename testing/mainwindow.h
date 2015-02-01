#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include <QListWidget>
#include <QLayout>
#include <QDockWidget>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void open();
    bool save();
    void newFile();
    bool saveAs();
    void copy();
   // void documentWasModified();

private:
    void createActions();
    void createMenus();
    bool isSave();
    bool saveFile(const QString &fileName);
    void loadFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void setDocks();
    void populateList();
    Ui::MainWindow *ui;
    QGridLayout * layout;
    QTextEdit * textEdit;
    QDockWidget * fileDock;
    QListWidget * fileList;
    QStringList * filter;
    QDir * curDir;
    QMenu * fileTool;
    QMenu * editTool;
    QString currentFileName;
    QAction * newAct;
    QAction * openAct;
    QAction * savAct;
    QAction * savAsAct;
    QAction * cutAct;
    QAction * copyAct;
    QAction * pasteAct;


};

#endif // MAINWINDOW_H
