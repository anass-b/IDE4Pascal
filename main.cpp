/*
*  main.cpp
*  ========
*
*/

#include <QApplication>
#include <QtGui>
#include "FenPrincipale.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	 
    FenPrincipale fenetre;
    fenetre.show();

    return app.exec();
}


