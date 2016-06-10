#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QProcess>
#include <stdlib.h>
#include "MdiChild.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private:
    MdiChild* activeMdiChild();
    QMdiSubWindow* findMdiChild(const QString& fileName);
    MdiChild* createMdiChild();
    void createUI();

private:
    QTextEdit* _textEdit;
    QTextEdit* _buildOutput;
    QString _filepath;
    QString _filename;
    QString _compilerPath;
    QString _workingDir;
    QProcess* _compilerProcess;
    QMdiArea* _mdiArea;
    QAction* _copyAction;
    QAction* _pasteAction;
    QAction* _cutAction;
    QAction* _selectAllAction;
    QAction* _undoAction;
    QAction* _redoAction;
    QAction* _actionSetDefault;
    QDialog* _configCmp;

public slots:
    void newFile();
    void about();
    void build();
    void runExecutable();
    void setCompilerPath();
    void changeFont();
    void undo();
    void redo();
    void selectAll();
    void copy();
    void paste();
    void cut();
    void open();
    void save();
    void saveAs();
};

#endif
