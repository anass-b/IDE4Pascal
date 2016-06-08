/*
*  MdiChild.cpp
*  ============
*
*/

 #include <QtGui>

 #include "MdiChild.h"

 MdiChild::MdiChild()
 {
     setAttribute(Qt::WA_DeleteOnClose);
	 setFont(QFont("Courier New", 10));
	 highlighter = new Highlighter(this->document());
     isUntitled = true;
 }

 void MdiChild::newFile()
 {
     static int sequenceNumber = 1;

     isUntitled = true;
     curFile = tr("file%1.pas").arg(sequenceNumber++);
     setWindowTitle(curFile + "[*]");

     connect(document(), SIGNAL(contentsChanged()),
             this, SLOT(documentWasModified()));
 }

 bool MdiChild::loadFile(const QString &fileName)
 {
     QFile file(fileName);
     if (!file.open(QFile::ReadOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Anas Pascal IDE"),
                              tr("Impossible de lire le fichier %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return false;
     }

     QTextStream in(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     setPlainText(in.readAll());
     QApplication::restoreOverrideCursor();

     setCurrentFile(fileName);

     connect(document(), SIGNAL(contentsChanged()),
             this, SLOT(documentWasModified()));

     return true;
 }

 bool MdiChild::save()
 {
     if (isUntitled) {
         return saveAs();
     } else {
         return saveFile(curFile);
     }
 }

 bool MdiChild::saveAs()
 {
     QString fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer sous..."),
                                                     curFile);
	gFileName = fileName;
     if (fileName.isEmpty())
         return false;

     return saveFile(fileName);
 }
 


 bool MdiChild::saveFile(const QString &fileName)
 {
	gFileName = fileName;
     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Anas Pascal IDE"),
                              tr("Impossible d'ecrire sur le fichier %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return false;
     }

     QTextStream out(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     out << toPlainText();
     QApplication::restoreOverrideCursor();

     setCurrentFile(fileName);
     return true;
 }

 QString MdiChild::userFriendlyCurrentFile()
 {
     return strippedName(curFile);
 }

 void MdiChild::closeEvent(QCloseEvent *event)
 {
     if (maybeSave()) {
         event->accept();
     } else {
         event->ignore();
     }
 }

 void MdiChild::documentWasModified()
 {
     setWindowModified(document()->isModified());
 }

 bool MdiChild::maybeSave()
 {
     if (document()->isModified()) {
         QMessageBox::StandardButton ret;
         ret = QMessageBox::warning(this, tr("Anas Pascal IDE"),
                      tr("'%1' a ete modifie.\n"
                         "Voulez vous enregistrer les changements ?")
                      .arg(userFriendlyCurrentFile()),
                      QMessageBox::Save | QMessageBox::Discard
                      | QMessageBox::Cancel);
         if (ret == QMessageBox::Save)
             return save();
         else if (ret == QMessageBox::Cancel)
             return false;
     }
     return true;
 }

 void MdiChild::setCurrentFile(const QString &fileName)
 {
     curFile = QFileInfo(fileName).canonicalFilePath();
     isUntitled = false;
     document()->setModified(false);
     setWindowModified(false);
     setWindowTitle(userFriendlyCurrentFile() + "[*]");
 }
 

 QString MdiChild::strippedName(const QString &fullFileName)
 {
     return QFileInfo(fullFileName).fileName();
 }




 
