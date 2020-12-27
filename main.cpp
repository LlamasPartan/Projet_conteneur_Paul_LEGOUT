/*!
	\file main.cpp
	\author LEGOUT Paul
	\date 03.01.2020
	\brief Lancement du main
*/

					/* Inclusion des bibliothèques */

#include <QApplication>
#include "Window.h"
#include "FirstWindow.h"
using namespace std;


/*!
	\fn int main
	\author LEGOUT Paul
	\date 03.01.2020
	\param int argc, le nombre de paramètres en entrée
	\param char **argv, les valeurs des paramètres en entrée
	\return Execute l'application si tout s'est bien déroulé
	\brief Lance l'application
*/
int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

    /* Création de l'objet Première fenêtre et affichage de celui-ci */
    FirstWindow firstWindow;
    firstWindow.show();

    return app.exec();
}
