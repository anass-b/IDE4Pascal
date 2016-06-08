/*
*  FenPrincipale.cpp
*  =================
*
*/


#include "FenPrincipale.h"

FenPrincipale::FenPrincipale()
{	
	setWindowTitle("Anas Pascal IDE");
    m_compilerPath = "fpc";
	
	// Creation des menus //   
	
	//Menu fichier
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");
		QAction *actionNouveau = menuFichier->addAction("&Nouveau"); 
		actionNouveau->setShortcut(QKeySequence("Ctrl+N"));
		actionNouveau->setIcon(QIcon("pixmap/icons/document-new.png"));
		actionNouveau->setStatusTip("Creer un nouveau fichier");
		
		QAction *actionOuvrir = menuFichier->addAction("&Ouvrir");
		actionOuvrir->setShortcut(QKeySequence("Ctrl+O"));
		actionOuvrir->setIcon(QIcon("pixmap/icons/document-open.png"));
		actionOuvrir->setStatusTip("Ouvre un fichier");
		
		QAction *actionEnregistrer = menuFichier->addAction("&Enregistrer");
		actionEnregistrer->setShortcut(QKeySequence("Ctrl+S"));
		actionEnregistrer->setIcon(QIcon("pixmap/icons/document-save.png"));
		actionEnregistrer->setStatusTip("Enregistre le fichier courant");
		
		QAction *actionEnregistrerSous = menuFichier->addAction("&Enregistrer sous ...");
		actionEnregistrerSous->setShortcut(QKeySequence("Ctrl+Alt+S"));
		actionEnregistrerSous->setIcon(QIcon("pixmap/icons/document-save-as.png"));
		actionEnregistrerSous->setStatusTip("Enregistre le fichier courant sous ...");
	
		menuFichier->addSeparator();
		
		QAction *actionQuitter = menuFichier->addAction("&Quitter");
		actionQuitter->setShortcut(QKeySequence("Alt+F4"));
		actionQuitter->setIcon(QIcon("pixmap/icons/system-log-out.png"));
		actionQuitter->setStatusTip("Quitte le programme");
    
	//Menu Edition
    QMenu *menuEdition = menuBar()->addMenu("&Edition");
		actionAnnuler = menuEdition->addAction("&Annuler");
		actionAnnuler->setShortcut(QKeySequence("Ctrl+Z"));
		actionAnnuler->setIcon(QIcon("pixmap/icons/edit-undo.png"));
		
		actionRetablir = menuEdition->addAction("&Retablir");
		actionRetablir->setShortcut(QKeySequence("Ctrl+Y"));
		actionRetablir->setIcon(QIcon("pixmap/icons/edit-redo.png"));
		
		menuEdition->addSeparator();
		
		actionCopier = menuEdition->addAction("&Copier");
		actionCopier->setShortcut(QKeySequence("Ctrl+C"));
		actionCopier->setIcon(QIcon("pixmap/icons/edit-copy.png"));
		
		actionCouper = menuEdition->addAction("&Couper");
		actionCouper->setShortcut(QKeySequence("Ctrl+X"));
		actionCouper->setIcon(QIcon("pixmap/icons/edit-cut.png"));
		
		actionColler = menuEdition->addAction("&Coller");
		actionColler->setShortcut(QKeySequence("Ctrl+V"));
		actionColler->setIcon(QIcon("pixmap/icons/edit-paste.png"));
		
		actionSelecTout = menuEdition->addAction("&Selectionner Tout");
		actionSelecTout->setShortcut(QKeySequence("Ctrl+A"));
		actionSelecTout->setIcon(QIcon("pixmap/icons/edit-select-all.png"));
	
	//Menu Format
    QMenu *menuFormat = menuBar()->addMenu("&Format");
		QAction *actionChangerFont = menuFormat->addAction("&Changer la police");
		actionChangerFont->setIcon(QIcon("pixmap/icons/preferences-desktop-font.png"));
	
	// Menu compiler
	QMenu *menuCompiler = menuBar()->addMenu("&Compiler");
		QAction *actionCompiler = menuCompiler->addAction("&Compiler");
		actionCompiler->setShortcut(QKeySequence("Ctrl+F9"));
		actionCompiler->setIcon(QIcon("pixmap/icons/preferences-system.png"));
		
		QAction *actionDebug = menuCompiler->addAction("&Executer");
		actionDebug->setShortcut(QKeySequence("Ctrl+F10"));
		actionDebug->setIcon(QIcon("pixmap/icons/media-playback-start.png"));
		
		QAction *actionSetCompiler = menuCompiler->addAction("&Configuer le compilateur");
		actionSetCompiler->setIcon(QIcon("pixmap/icons/applications-system.png"));
		
		QAction *actionSetDefault = menuCompiler->addAction("&Parametres par defaut ...");
		actionSetDefault->setIcon(QIcon("pixmap/icons/view-refresh.png"));

		QAction *actionSetCmpOp = menuCompiler->addAction("&Parametrer la compilation ...");
		actionSetCmpOp->setIcon(QIcon("pixmap/icons/utilities-system-monitor.png"));		
		

	//Menu aide
	QMenu *menuAide = menuBar()->addMenu("&?");
		QAction *actionApropos = menuAide->addAction("&A propos");
		actionApropos->setIcon(QIcon("pixmap/icons/help-browser.png"));
		
    // Création de la barre d'outils //
	
	//Barre d'outils fichier
    QToolBar *toolBarFichier = addToolBar("Fichier");
		toolBarFichier->addAction(actionNouveau);
		toolBarFichier->addAction(actionOuvrir);
		toolBarFichier->addAction(actionEnregistrer);
		toolBarFichier->addAction(actionEnregistrerSous);
		
  
	//Barre d'outils Edition
	QToolBar *toolBarEdition = addToolBar("Edition");
		toolBarEdition->addAction(actionAnnuler);
		toolBarEdition->addAction(actionRetablir);
	    toolBarEdition->addAction(actionCopier);
		toolBarEdition->addAction(actionCouper);
		toolBarEdition->addAction(actionColler);
	
	//Barre d'outils compiler
	QToolBar *toolBarCompiler = addToolBar("Compiler");
    toolBarCompiler->addAction(actionCompiler);
	toolBarCompiler->addAction(actionDebug);
	toolBarCompiler->addAction(actionSetCompiler);
	toolBarCompiler->addAction(actionSetDefault);
	toolBarCompiler->addAction(actionSetCmpOp);
	
	
	//Barre d'etat
	QStatusBar *barreEtat = statusBar();
	barreEtat->showMessage("Pret");

	//Zone centrale
	zoneCentrale = new QMdiArea;	
	setCentralWidget(zoneCentrale);
	
	//Dock
	QDockWidget *dock = new QDockWidget("Resultat de la compilation", this);
	addDockWidget(Qt::BottomDockWidgetArea, dock);
	
	QWidget *contenuDock = new QWidget;
	dock->setWidget(contenuDock);
	
	m_sortieCompilation = new QTextEdit(this);
	m_sortieCompilation->setObjectName(QString::fromUtf8("sortieCompilation"));
    //m_sortieCompilation->setGeometry(QRect(0, 0, 10, 10));
	m_sortieCompilation->setFont(QFont("Courier New", 10, QFont::Bold, false));
	
	
	QVBoxLayout *dockLayout = new QVBoxLayout;
	dockLayout->setSpacing(0);
	
	dockLayout->addWidget(m_sortieCompilation);
	
	contenuDock->setLayout(dockLayout);

	m_cmpProcess = new QProcess;
	m_debugExec = new QProcess;
	m_file_exec = new QProcess;
	
	//Les actions
    connect(actionQuitter,     SIGNAL(triggered()), qApp,     SLOT(  quit()            ));
	connect(actionNouveau, SIGNAL(triggered()),     this,     SLOT(  newFile()   ));
	connect(actionOuvrir, SIGNAL(triggered()),      this,     SLOT(  open()   ));
	connect(actionEnregistrer, SIGNAL(triggered()), this,     SLOT(  save()   ));
	connect(actionEnregistrerSous, SIGNAL(triggered()), this, SLOT(  saveAs()   ));
	connect(actionCompiler,    SIGNAL(triggered()), this,     SLOT(  compilerFichier() ));
	connect(actionDebug,       SIGNAL(triggered()), this,     SLOT(  executerFichier() ));
	connect(actionApropos,     SIGNAL(triggered()), this, 	  SLOT(  aPropos() ));
	connect(actionSetCompiler, SIGNAL(triggered()), this,     SLOT(  setCompilerPath() ));
	connect(actionAnnuler,     SIGNAL(triggered()), this,     SLOT(  undo()   ));
	connect(actionRetablir,    SIGNAL(triggered()), this,     SLOT(  redo()   ));
	connect(actionCopier,      SIGNAL(triggered()), this,     SLOT(  copy()   ));
	connect(actionColler,      SIGNAL(triggered()), this,     SLOT(  paste()   ));
	connect(actionCouper,      SIGNAL(triggered()), this,     SLOT(  cut()   ));
	connect(actionSelecTout,   SIGNAL(triggered()), this,     SLOT(  selectAll()   ));
	connect(actionChangerFont, SIGNAL(triggered()), this,     SLOT(  changerPolice()   ));
	connect(actionSetDefault, SIGNAL(triggered()),  this,     SLOT(  paramParDef()   ));
	connect(actionSetCmpOp, SIGNAL(triggered()),    this,     SLOT(  setCompilerOptions()   ));
}

/**************************** cut ****************************/
void FenPrincipale::cut()
 {
     if (activeMdiChild())
         activeMdiChild()->cut();
 }

 /**************************** copy ****************************/
 void FenPrincipale::copy()
 {
     if (activeMdiChild())
         activeMdiChild()->copy();
 }

 /**************************** paste ****************************/
 void FenPrincipale::paste()
 {
     if (activeMdiChild())
         activeMdiChild()->paste();
 }
 
 /**************************** selectAll ****************************/
 void FenPrincipale::selectAll()
 {
     if (activeMdiChild())
         activeMdiChild()->selectAll();
 }
 /**************************** undo ****************************/
 void FenPrincipale::undo()
 {
     if (activeMdiChild())
         activeMdiChild()->undo();
 }
 
 /**************************** redo ****************************/
 void FenPrincipale::redo()
 {
     if (activeMdiChild())
         activeMdiChild()->redo();
 }

 /**************************** newFile ****************************/
void FenPrincipale::newFile()
 {
     MdiChild *child = createMdiChild();
     child->newFile();
     child->show();
	 
	
 }
 
 /**************************** createMdiChild ****************************/
 MdiChild *FenPrincipale::createMdiChild() 
 {
     MdiChild *child = new MdiChild;
    zoneCentrale->addSubWindow(child);

     connect(child, SIGNAL(copyAvailable(bool)),
             actionCouper, SLOT(setEnabled(bool)));
     connect(child, SIGNAL(copyAvailable(bool)),
             actionCopier, SLOT(setEnabled(bool)));

     return child;
 }
 

/**************************** open ****************************/
void FenPrincipale::open() 
 {
 
     QString fileName = QFileDialog::getOpenFileName(this);
     if (!fileName.isEmpty()) {
		 m_nomFichier = fileName;
         QMdiSubWindow *existing = findMdiChild(fileName);
         if (existing) {
             zoneCentrale->setActiveSubWindow(existing);
             return;
         }

        MdiChild *child = createMdiChild();
         if (child->loadFile(fileName)) {
             statusBar()->showMessage(tr("Fichier charge"), 2000);
             child->show();
         } else {
             child->close();
         }
     }
 }
 
 /**************************** findMdiChild ****************************/
 
 QMdiSubWindow *FenPrincipale::findMdiChild(const QString &fileName)
 {
     QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

     foreach (QMdiSubWindow *window, zoneCentrale->subWindowList()) {
         MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
         if (mdiChild->currentFile() == canonicalFilePath)
             return window;
     }
     return 0;
 }

 /**************************** activeMdiChild ****************************/
MdiChild *FenPrincipale::activeMdiChild()
 {
     if (QMdiSubWindow *activeSubWindow = zoneCentrale->activeSubWindow())
         return qobject_cast<MdiChild *>(activeSubWindow->widget());
     return 0;
 }
 
 /**************************** save ****************************/
 void FenPrincipale::save()
 {
	 if (activeMdiChild() && activeMdiChild()->save())
         statusBar()->showMessage(tr("Fichier enregistre"), 2000);
	m_nomFichier = activeMdiChild()->getFileName();
 }
 
 /**************************** saveAs ****************************/
 void FenPrincipale::saveAs()
 {
	
     if (activeMdiChild() && activeMdiChild()->saveAs())
         statusBar()->showMessage(tr("Fichier enregistre"), 2000);
 }
 
/**************************** aPropos ****************************/
void FenPrincipale::aPropos()  
{
	 QMessageBox::about(this, "A propos de", "Anas Pascal IDE v 0.0.1 \nEcrit par Anas Bouassaba");
}

/**************************** setCompilerPath ****************************/
void FenPrincipale::setCompilerPath()
{
	bool ok = false;
	QString strChoix;
	strChoix = QInputDialog::getText(this, "Configuration du compilateur", 
						"Chemain actuelle: " + m_compilerPath + "\n Nouveau chemain:", QLineEdit::Normal, QString(), &ok);
	
	if (ok && !strChoix.isEmpty())
		m_compilerPath = strChoix;
}

/**************************** setCompilerOptions ****************************/

void FenPrincipale::setCompilerOptions()
{
	configCmp = new QDialog(this);
	configCmp->setFixedSize(400,250);
	configCmp->setWindowTitle("Parametrer la compilation");
	
	QLabel *image = new QLabel(configCmp);
	image->setPixmap(QPixmap("pixmap/icons/emblem-system.png"));
	image->move(10, 10);
	
	QLabel *titre = new QLabel("Parametres de la compilation", configCmp);
	titre->setFont(QFont("Verdana", 10));
	titre->move(50,15);
	
	/******** GroupBox ********/
	QGroupBox *groupBox = new QGroupBox("Ajouter des arguments a la compilation", configCmp); 
	groupBox->move(10,60);		
	
	op1 = new QLineEdit;
	op1->setText(m_cmpOp1);
	op2 = new QLineEdit;
	op2->setText(m_cmpOp2);
	
	
	QFormLayout *layout = new QFormLayout;
	layout->addRow("Arguments avant le nom du fichier:", op1);
	layout->addRow("Arguments apres le nom du fichier:", op2);
	groupBox->setLayout(layout);
	/*************************/
	
	QPushButton *ok = new QPushButton("Ok");
	connect(ok, SIGNAL(clicked()), this, SLOT(  setCmpArgs() ));
	QPushButton *annuler = new QPushButton("Annuler");
	connect(annuler, SIGNAL(clicked()), configCmp, SLOT(  accept() ));
	
	QWidget *buttonsWidget = new QWidget(configCmp);
	buttonsWidget->move(220,200);
	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addWidget(ok);
	hlayout->addWidget(annuler);
	buttonsWidget->setLayout(hlayout);
	
	configCmp->show();
}

/**************************** setCmpArgs ****************************/

void FenPrincipale::setCmpArgs()
{
	m_cmpOp1 = op1->text();
	m_cmpOp2 = op2->text();
	//QMessageBox::information(this, "test", cmpOp1 + " et " + cmpOp2);
	configCmp->accept();
}

/**************************** changerPolice ****************************/ 

void FenPrincipale::changerPolice()
{
	bool ok = false;
 
	if(activeMdiChild())
	{
		QFont cEditFont = QFontDialog::getFont(&ok, activeMdiChild()->font(), this, "Choisissez une police");
		
	    if (ok)
	    {
	        activeMdiChild()->setFont(cEditFont);
		}
		
	}
}

/**************************** paramParDef ****************************/

void FenPrincipale::paramParDef()
{
    m_compilerPath = "fpc";
	QMessageBox::information(this, "Parametres par defaut", "Tous les parametres ont ete restores.");
}

/**************************** compilerFichier****************************/

void FenPrincipale::compilerFichier()
{
	int strl=0, strlf=0, i=0;
	QString workDir;
	QChar c;
	
	
	if (activeMdiChild() && activeMdiChild()->save())
         statusBar()->showMessage(tr("File saved"), 2000);
	
	pname = "";
	fname = "";
	
	if (activeMdiChild())
         m_nomFichier = activeMdiChild()->getFileName();
		 
	pname = m_nomFichier;
	m_sortieCompilation->clear();
     
	fname = QFileInfo(pname).fileName();
	strlf = fname.size();
	strl = pname.size();
	i = strl;
	c = '0';
	while(c != '/')
	{
		c = pname[i];
 		
		if(c == '/')
		{
			pname.replace(strl - strlf - 1, strlf + 1, ""); 
		}
		i--;
	}
	
    QString workingDir = m_cmpProcess->workingDirectory();
	m_cmpProcess->setWorkingDirectory(pname);

    //QString compileString = m_compilerPath + " " + m_cmpOp1 + " " + fname + " " + m_cmpOp2 + " > BUILDLOG.TXT";
    m_cmpProcess->setStandardOutputFile("BUILDLOG.TXT");

    m_cmpProcess->setProcessChannelMode(QProcess::MergedChannels);
    m_cmpProcess->start("fpc", QStringList() << fname);

    m_cmpProcess->waitForFinished();
    QString errorStr = m_cmpProcess->errorString();
	
	workDir = m_cmpProcess->workingDirectory();
	
	QFile file(workDir + "/BUILDLOG.TXT");
	 if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		 return;
	
	 while (!file.atEnd()) 
	 {
		 QString line = file.readLine();
		 m_sortieCompilation->append(line);
	 }

}

/**************************** executerFichier****************************/
void FenPrincipale::executerFichier()
{
	QString strExeFile, tmpp;
	int strl=0,strl2=0, i=0, p=0;
	strl = fname.size();
	strExeFile = fname;
	strExeFile.remove(strl-4,4);

	//************* Transformer le path du fichier en un chemain Win valide *********
	tmpp = pname;
	strl =  tmpp.size();
	while(i<strl)
	{
		p= tmpp.indexOf('/',i);
		if(p != -1)
		{
			 tmpp.replace(p, 1, '\\');
			i=p;
		}
		i++;
	}
	
	//***************************************************************************
	
	strl2 = fname.size();
	strExeFile = fname;
	strExeFile.remove(strl2-4,4);
	
     QFile file("batfile.bat");
     if (!file.open(QFile::WriteOnly | QFile::Text)) 
	 {
         QMessageBox::warning(this, tr("Anas Pascal IDE"),
                              tr("Erreur lors de la preparation de l'execution du fichier.")
                              .arg(fname)
                              .arg(file.errorString()));
     }

	 QTextStream out(&file);
	 out << "path=%path%;"+tmpp + "\n" + "cls" + "\n" + strExeFile;
	 file.close();
	system("batfile.bat");
	

}













