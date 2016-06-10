/*
*  MainWindow.h
*  ===============
*
*/

#ifndef HEADER_MainWindow
#define HEADER_MainWindow

#include <QtWidgets>
#include <QProcess>
#include <stdlib.h>
#include "MdiChild.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
	public:
        MainWindow();
		

	private:
		MdiChild * activeMdiChild();   
		QMdiSubWindow *findMdiChild(const QString &fileName);
		QTextEdit *m_textEdit, *m_sortieCompilation;
		QString m_nomFichier, m_titreFenetre, m_code, m_compilerPath;
		QProcess *m_cmpProcess, *m_debugExec, *m_file_exec;
		QMdiArea *zoneCentrale;
		QStringList arguments;
		QString program;
		QString m_line;
		QString m_currentDir;
		QString fname;
		QString pname;
		QLineEdit *op1;
		QLineEdit *op2;
		QString m_cmpOp1;
		QString m_cmpOp2;
		QAction *actionCopier;
		QAction *actionColler;
		QAction *actionCouper;
		QAction *actionSelecTout;
		QAction *actionAnnuler;
		QAction *actionRetablir;
		QAction *actionSetDefault;
		QDialog *configCmp;

	public slots :
		void newFile();
		void aPropos();
		void compilerFichier();
		void executerFichier();
		void setCompilerPath();
		void paramParDef();
		void setCompilerOptions();
		void changerPolice();
		void setCmpArgs();
		MdiChild * createMdiChild();
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





