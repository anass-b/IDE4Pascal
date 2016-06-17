#include "MainWindow.h"

#ifdef _WIN32
#define DEFAULT_COMPILER_PATH "C:/FPC/3.0.0/bin/i386-win32/ppcrossx64.exe"
#elif __APPLE__
#define DEFAULT_COMPILER_PATH "/usr/local/bin/fpc"
#else
#define DEFAULT_COMPILER_PATH "/usr/bin/fpc"
#endif

MainWindow::MainWindow()
{
    setWindowTitle("IDE4Pascal");
    _compilerPath = DEFAULT_COMPILER_PATH;
    _compilerProcess = new QProcess;

    createUI();
}

void MainWindow::createUI()
{
    // File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");

    QAction* newAction = fileMenu->addAction("&New");
    newAction->setShortcut(QKeySequence("Ctrl+N"));
    newAction->setIcon(QIcon(":/res/document-new.png"));
    newAction->setStatusTip("Create new file");

    QAction* openAction = fileMenu->addAction("&Open");
    openAction->setShortcut(QKeySequence("Ctrl+O"));
    openAction->setIcon(QIcon(":/res/document-open.png"));
    openAction->setStatusTip("Open file");

    QAction* saveAction = fileMenu->addAction("&Save");
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    saveAction->setIcon(QIcon(":/res/document-save.png"));
    saveAction->setStatusTip("Save file");

    QAction* saveAsAction = fileMenu->addAction("&Save As...");
    saveAsAction->setShortcut(QKeySequence("Ctrl+Alt+S"));
    saveAsAction->setIcon(QIcon(":/res/document-save-as.png"));
    saveAsAction->setStatusTip("Save file as...");

    fileMenu->addSeparator();

    QAction* quitAction = fileMenu->addAction("&Quit");
    quitAction->setShortcut(QKeySequence("Alt+F4"));
    quitAction->setIcon(QIcon(":/res/system-log-out.png"));
    quitAction->setStatusTip("Quit application");

    // Edit menu
    QMenu* editMenu = menuBar()->addMenu("&Edit");

    _undoAction = editMenu->addAction("&Undo");
    _undoAction->setShortcut(QKeySequence("Ctrl+Z"));
    _undoAction->setIcon(QIcon(":/res/edit-undo.png"));

    _redoAction = editMenu->addAction("&Redo");
    _redoAction->setShortcut(QKeySequence("Ctrl+Y"));
    _redoAction->setIcon(QIcon(":/res/edit-redo.png"));

    editMenu->addSeparator();

    _copyAction = editMenu->addAction("&Copy");
    _copyAction->setShortcut(QKeySequence("Ctrl+C"));
    _copyAction->setIcon(QIcon(":/res/edit-copy.png"));

    _cutAction = editMenu->addAction("&Cut");
    _cutAction->setShortcut(QKeySequence("Ctrl+X"));
    _cutAction->setIcon(QIcon(":/res/edit-cut.png"));

    _pasteAction = editMenu->addAction("&Paste");
    _pasteAction->setShortcut(QKeySequence("Ctrl+V"));
    _pasteAction->setIcon(QIcon(":/res/edit-paste.png"));

    _selectAllAction = editMenu->addAction("&Select All");
    _selectAllAction->setShortcut(QKeySequence("Ctrl+A"));
    _selectAllAction->setIcon(QIcon(":/res/edit-select-all.png"));

    // Format menu
    QMenu* formatMenu = menuBar()->addMenu("&Format");

    QAction* changeFontAction = formatMenu->addAction("&Change font");
    changeFontAction->setIcon(QIcon(":/res/preferences-desktop-font.png"));

    // Build menu
    QMenu* menuCompiler = menuBar()->addMenu("&Build");

    QAction* actionCompiler = menuCompiler->addAction("&Build file");
    actionCompiler->setShortcut(QKeySequence("Ctrl+F9"));
    actionCompiler->setIcon(QIcon(":/res/applications-system.png"));

    QAction* runExecutableAction = menuCompiler->addAction("&Run");
    runExecutableAction->setShortcut(QKeySequence("Ctrl+F10"));
    runExecutableAction->setIcon(QIcon(":/res/media-playback-start.png"));

    QAction* setCompilerAction = menuCompiler->addAction("&Set compiler path");
    setCompilerAction->setIcon(QIcon(":/res/preferences-system.png"));

    // About menu
    QMenu* helpMenu = menuBar()->addMenu("&About");

    QAction* aboutAction = helpMenu->addAction("&About");
    aboutAction->setIcon(QIcon(":/res/help-browser.png"));

    // File toolbar
    QToolBar* fileToolbar = addToolBar("File");
    fileToolbar->addAction(newAction);
    fileToolbar->addAction(openAction);
    fileToolbar->addAction(saveAction);
    fileToolbar->addAction(saveAsAction);

    // Edit toolbar
    QToolBar* editToolbar = addToolBar("Edit");
    editToolbar->addAction(_undoAction);
    editToolbar->addAction(_redoAction);
    editToolbar->addAction(_copyAction);
    editToolbar->addAction(_cutAction);
    editToolbar->addAction(_pasteAction);

    // Build toolbar
    QToolBar* buildToolbar = addToolBar("Build");
    buildToolbar->addAction(actionCompiler);
    buildToolbar->addAction(runExecutableAction);
    buildToolbar->addAction(setCompilerAction);

    // Statusbar
    QStatusBar* statusbar = statusBar();
    statusbar->showMessage("Ready");

    // Central Widget
    createTabUi();

    // Dock
    QDockWidget* dock = new QDockWidget("Build output", this);
    addDockWidget(Qt::BottomDockWidgetArea, dock);

    QWidget* dockContent = new QWidget;
    dock->setWidget(dockContent);

    _buildOutput = new QTextEdit(this);
    _buildOutput->setReadOnly(true);
#ifdef __APPLE__
    int outputFontSize = 12;
    QString outputFontFamily = "Menlo";
#elif _WIN32
    int outputFontSize = 11;
    QString outputFontFamily = "Consolas";
#else
    int outputFontSize = 10;
    QString outputFontFamily = "Courier New";
#endif
    _buildOutput->setFont(QFont(outputFontFamily, outputFontSize, QFont::Bold, false));

    QVBoxLayout* dockLayout = new QVBoxLayout;
    dockLayout->setSpacing(0);
    dockLayout->addWidget(_buildOutput);

    dockContent->setLayout(dockLayout);

    // Actions
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(actionCompiler, SIGNAL(triggered()), this, SLOT(build()));
    connect(runExecutableAction, SIGNAL(triggered()), this, SLOT(runExecutable()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(setCompilerAction, SIGNAL(triggered()), this, SLOT(setCompilerPath()));
    connect(_undoAction, SIGNAL(triggered()), this, SLOT(undo()));
    connect(_redoAction, SIGNAL(triggered()), this, SLOT(redo()));
    connect(_copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    connect(_pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
    connect(_cutAction, SIGNAL(triggered()), this, SLOT(cut()));
    connect(_selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));
    connect(changeFontAction, SIGNAL(triggered()), this, SLOT(changeFont()));
}

void MainWindow::createTabUi()
{
    _tabWidget = new QTabWidget;
    _tabWidget->setMovable(true);
    _tabWidget->setDocumentMode(true);
    _tabWidget->setTabsClosable(true);

    connect(_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    setCentralWidget(_tabWidget);
}

void MainWindow::closeTab(int index)
{
    _tabWidget->removeTab(index);
}

void MainWindow::cut()
{
    Editor* tab = activeTab();
    if (tab) {
        tab->cut();
    }
}

void MainWindow::copy()
{
    Editor* tab = activeTab();
    if (tab) {
        tab->copy();
    }
}

void MainWindow::paste()
{
    Editor* tab = activeTab();
    if (tab) {
        tab->paste();
    }
}

void MainWindow::selectAll()
{
    Editor* tab = activeTab();
    if (tab) {
        tab->selectAll();
    }
}

void MainWindow::undo()
{
    Editor* tab = activeTab();
    if (tab) {
        tab->undo();
    }
}

void MainWindow::redo()
{
    Editor* tab = activeTab();
    if (tab) {
        tab->redo();
    }
}

QString MainWindow::newTabName()
{
    static int sequenceNumber = 1;
    return tr("file%1.pas").arg(sequenceNumber++);
}

void MainWindow::newFile()
{
    createTab();
}

Editor* MainWindow::createTab(const QString& name)
{
    QString tabTitle = name.length() == 0 ? newTabName() : name;

    Editor* child = new Editor;
    child->setGeneratedFilename(tabTitle);

    int index = _tabWidget->addTab(child, tabTitle);
    _tabWidget->setCurrentIndex(index);

    return child;
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        _filepath = fileName;

        Editor* child = createTab(QFileInfo(_filepath).fileName());
        if (child->loadFile(fileName)) {
            statusBar()->showMessage(tr("File loaded"), 2000);
            child->show();
        }
        else {
            child->close();
        }
    }
}

Editor* MainWindow::activeTab()
{
    return (Editor*)_tabWidget->currentWidget();
}

void MainWindow::save()
{
    if (activeTab() && activeTab()->save()) {
        statusBar()->showMessage(tr("File saved"), 2000);
    }
}

void MainWindow::saveAs()
{
    if (activeTab() && activeTab()->saveAs())
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::about()
{
    QMessageBox::about(this, "About", "IDE4Pascal v0.0.1");
}

void MainWindow::setCompilerPath()
{
    bool ok = false;
    QString res = QInputDialog::getText(this, "Set compiler path", "Current path: " + _compilerPath,
        QLineEdit::Normal, QString(), &ok);
    if (ok && !res.isEmpty())
        _compilerPath = res;
}

void MainWindow::changeFont()
{
    bool ok = false;
    if (activeTab()) {
        QFont cEditFont = QFontDialog::getFont(&ok, activeTab()->font(), this, "Change font");
        if (ok) {
            activeTab()->setFont(cEditFont);
        }
    }
}

bool MainWindow::build()
{
    if (!activeTab())
        return false;

    _buildOutput->clear();

    if (activeTab() && activeTab()->save())
        statusBar()->showMessage(tr("File saved"), 2000);

    _filepath = activeTab()->filePath();
    _filename = QFileInfo(_filepath).fileName();
    _workingDir = QFileInfo(_filepath).dir().absolutePath();

    QString buildLogFilepath = _workingDir + QDir::separator() + "BUILDLOG.TXT";

    _compilerProcess->setWorkingDirectory(_workingDir);
    _compilerProcess->setStandardOutputFile(buildLogFilepath);
    _compilerProcess->setProcessChannelMode(QProcess::MergedChannels);
    _compilerProcess->start(_compilerPath, QStringList() << _filepath);
    _compilerProcess->waitForFinished();

    QString errorStr = _compilerProcess->errorString();
    if (errorStr.length() > 0) {
        QFile buildLogFile(buildLogFilepath);
        if (buildLogFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            while (!buildLogFile.atEnd()) {
                QString line = buildLogFile.readLine();
                _buildOutput->append(line);
            }
        }
    }

    int exitCode = _compilerProcess->exitCode();
    if (exitCode == 0) {
        statusBar()->showMessage(tr("Build succeeded"), 2000);
        return true;
    }
    else if (exitCode == 1) {
        statusBar()->showMessage(tr("Build failed"), 2000);
        return false;
    }

    return false;
}

QString MainWindow::exeFilePath()
{
#ifdef _WIN32
    QString exe = _workingDir + QDir::separator() + QFileInfo(_filepath).baseName() + ".exe";
    exe.replace("\\", "/");
#else
    QString exe = _workingDir + QDir::separator() + QFileInfo(_filepath).baseName();
#endif

    return exe;
}

void MainWindow::platformSpecificRunExe(QString exe)
{
#ifdef __APPLE__
    QProcess proc;
    proc.startDetached("/Applications/Utilities/Terminal.app/Contents/MacOS/Terminal",
        QStringList() << exe);
#elif _WIN32
    QProcess proc;
    proc.startDetached(exe);
#else
    QProcess proc;
    proc.startDetached("xterm", QStringList() << "-e"
                                              << "sh"
                                              << "-c" << exe);
#endif
}

void MainWindow::runExecutable()
{
    if (!activeTab())
        return;

    QString exe = exeFilePath();
    QFile exeFile(exe);

    // TODO: We need to build only when there are changes
    if (build()) {
        if (exeFile.exists()) {
            platformSpecificRunExe(exe);
        }
    }
}
