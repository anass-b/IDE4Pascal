#include <QApplication>
#include <QtWidgets>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            QSize(app.desktop()->geometry().width()/2, app.desktop()->geometry().height()/2),
            app.desktop()->availableGeometry()
        )
    );
    window.show();

    return app.exec();
}
