/*
*  MdiChild.cpp
*  ============
*
*/

#include <QtWidgets>
#include "MdiChild.h"

MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);

#ifdef _WIN32
    setFont(QFont("Courier New", 12));
#elif __APPLE__
    setFont(QFont("Menlo", 12));
#else
    setFont(QFont("Courier New", 12));
#endif

    _highlighter = new Highlighter(this->document());
    _isUntitled = true;
}

void MdiChild::newFile()
{
    static int sequenceNumber = 1;

    _isUntitled = true;
    _currentFile = tr("file%1.pas").arg(sequenceNumber++);
    setWindowTitle(_currentFile + "[*]");

    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
}

bool MdiChild::loadFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,
            tr("IDE4Pascal"),
            tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);

    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

    return true;
}

bool MdiChild::save()
{
    if (_isUntitled) {
        return saveAs();
    }
    else {
        return saveFile(_currentFile);
    }
}

bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As..."), _currentFile);
    _filename = fileName;

    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MdiChild::saveFile(const QString& fileName)
{
    _filename = fileName;
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,
            tr("IDE4Pascal"),
            tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
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
    return strippedName(_currentFile);
}

void MdiChild::closeEvent(QCloseEvent* event)
{
    if (maybeSave()) {
        event->accept();
    }
    else {
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
        ret = QMessageBox::warning(this,
            tr("IDE4Pascal"),
            tr("'%1' changed.\n"
               "Would you like to save the changes ?")
                .arg(userFriendlyCurrentFile()),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return save();
        }
        else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }

    return true;
}

void MdiChild::setCurrentFile(const QString& fileName)
{
    _currentFile = QFileInfo(fileName).canonicalFilePath();
    _isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MdiChild::strippedName(const QString& fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
