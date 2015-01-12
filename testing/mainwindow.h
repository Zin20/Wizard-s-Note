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

private:
    void createActions();
    void createMenus();
    bool isSave();
    bool saveFile(const QString &fileName);
    void loadFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    Ui::MainWindow *ui;
    QTextEdit * textEdit;
    QMenu * fileTool;
    QString currentFileName;
    QAction * newAct;
    QAction * openAct;
    QAction * savAct;
    QAction * savAsAct;


};

#endif // MAINWINDOW_H
