#include "MainWindow.h"

#ifdef _WIN32
    #define DEFAULT_COMPILER_PATH "C:/FPC/3.0.0/bin/i386-win32/ppcrossx64.exe"
#elif __APPLE__
    #define DEFAULT_COMPILER_PATH "/usr/local/bin/fpc"
#elif
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

    _selectAllAction = editMenu->addAction("&Select Alll");
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

    // MDI
    _mdiArea = new QMdiArea;
    setCentralWidget(_mdiArea);

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

void MainWindow::cut()
{
    if (activeMdiChild())
        activeMdiChild()->cut();
}

void MainWindow::copy()
{
    if (activeMdiChild())
        activeMdiChild()->copy();
}

void MainWindow::paste()
{
    if (activeMdiChild())
        activeMdiChild()->paste();
}

void MainWindow::selectAll()
{
    if (activeMdiChild())
        activeMdiChild()->selectAll();
}

void MainWindow::undo()
{
    if (activeMdiChild())
        activeMdiChild()->undo();
}

void MainWindow::redo()
{
    if (activeMdiChild())
        activeMdiChild()->redo();
}

void MainWindow::newFile()
{
    MdiChild* child = createMdiChild();
    child->newFile();
    child->show();
    _mdiArea->activeSubWindow()->resize(600, 400);
}

MdiChild* MainWindow::createMdiChild()
{
    MdiChild* child = new MdiChild;
    _mdiArea->addSubWindow(child);

    connect(child, SIGNAL(copyAvailable(bool)), _cutAction, SLOT(setEnabled(bool)));
    connect(child, SIGNAL(copyAvailable(bool)), _copyAction, SLOT(setEnabled(bool)));

    return child;
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        _filepath = fileName;
        QMdiSubWindow* existing = findMdiChild(fileName);
        if (existing) {
            _mdiArea->setActiveSubWindow(existing);
            return;
        }

        MdiChild* child = createMdiChild();
        if (child->loadFile(fileName)) {
            statusBar()->showMessage(tr("File loaded"), 2000);
            child->show();
            _mdiArea->activeSubWindow()->resize(600, 400);
        }
        else {
            child->close();
        }
    }
}

QMdiSubWindow* MainWindow::findMdiChild(const QString& fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow* window, _mdiArea->subWindowList()) {
        MdiChild* mdiChild = qobject_cast<MdiChild*>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

MdiChild* MainWindow::activeMdiChild()
{
    if (QMdiSubWindow* activeSubWindow = _mdiArea->activeSubWindow())
        return qobject_cast<MdiChild*>(activeSubWindow->widget());
    return 0;
}

void MainWindow::save()
{
    if (activeMdiChild() && activeMdiChild()->save()) {
        statusBar()->showMessage(tr("File saved"), 2000);
    }
}

void MainWindow::saveAs()
{
    if (activeMdiChild() && activeMdiChild()->saveAs())
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
    if (activeMdiChild()) {
        QFont cEditFont = QFontDialog::getFont(&ok, activeMdiChild()->font(), this, "Change font");
        if (ok) {
            activeMdiChild()->setFont(cEditFont);
        }
    }
}

void MainWindow::build()
{
    if (!activeMdiChild())
        return;

    _buildOutput->clear();

    if (activeMdiChild() && activeMdiChild()->save())
        statusBar()->showMessage(tr("File saved"), 2000);

    _filepath = activeMdiChild()->getFileName();
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
        if (!buildLogFile.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        while (!buildLogFile.atEnd()) {
            QString line = buildLogFile.readLine();
            _buildOutput->append(line);
        }
    }
}

QString MainWindow::exeFilePath()
{
#ifdef __APPLE__
    QString exe = _workingDir + QDir::separator() + QFileInfo(_filepath).baseName();
#elif _WIN32
    QString exe = _workingDir + QDir::separator() + QFileInfo(_filepath).baseName() + ".exe";
    exe.replace("\\", "/");
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
#endif
}

void MainWindow::runExecutable()
{
    if (!activeMdiChild())
        return;

    QString exe = exeFilePath();
    QFile exeFile(exe);

    // TODO: We need to build only when there are changes
    build();

    if (exeFile.exists()) {
        platformSpecificRunExe(exe);
    }
}
