#ifndef MDICHILD_H
#define MDICHILD_H

#include <QTextEdit>
#include "Highlighter.h"
#include "codeeditor.h"

class MdiChild : public CodeEditor {
    Q_OBJECT

public:
    MdiChild();
    void newFile();
    bool loadFile(const QString& fileName);
    bool save();
    bool saveAs();
    QString getFileName() { return _filename; }
    bool saveFile(const QString& fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return _currentFile; }

private:
    bool maybeSave();
    void setCurrentFile(const QString& fileName);
    QString strippedName(const QString& fullFileName);

private:
    QString _filename;
    QString _currentFile;
    Highlighter* _highlighter;
    bool _isUntitled;

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void documentWasModified();
};

#endif
