/*
*  MdiChild.h
*  ==========
*
*/

 #ifndef MDICHILD_H
 #define MDICHILD_H

 #include <QTextEdit>
 #include "Highlighter.h"
#include "codeeditor.h"

 class MdiChild : public CodeEditor
 {
     Q_OBJECT

 public:
     MdiChild();

     void newFile();
     bool loadFile(const QString &fileName);
     bool save();
     bool saveAs();
	  QString getFileName() {return gFileName; }
     bool saveFile(const QString &fileName);
     QString userFriendlyCurrentFile();
     QString currentFile() { return curFile; }
	 

 protected:
     void closeEvent(QCloseEvent *event);

 private slots:
     void documentWasModified();

 private:
     bool maybeSave();
     void setCurrentFile(const QString &fileName);
     QString strippedName(const QString &fullFileName);
	QString gFileName;
     QString curFile;
	 Highlighter *highlighter;
     bool isUntitled;
 };

 #endif

 
 
 
 
 
 
