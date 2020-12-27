#ifndef WINDOW_H
#define WINDOW_H

/*!
	\file Window.h
	\author LEGOUT Paul
	\date 17.01.2020
	\brief Fenetre contenant les informations du porte-conteneurs et permet, via un bouton, le lancement du remplissage du paquebot avec un suivi graphique de la progression.
*/

					/* Inclusion des librairies */

/* Inclusion des widgets */
#include <QWidget>
/* Inclusion des boutons */
#include <QPushButton>
/* Inclusion des labels */
#include <QLabel>
/* Inclusion des layouts verticaux et horizontaux */
#include <QVBoxLayout>
#include <QHBoxLayout>
/* Inclusion des layouts grilles */
#include <QGridLayout>
/* Inclusion des barres de progression */
#include <QProgressBar>
/* Inclusion des fonctions mathématiques */
#include <math.h>
/* Inclusion des timers */
#include <QTimer>
/* Inclusion des boites de dialogue affichant un message */
#include <QMessageBox>

using namespace std;

					
					/* Définition de la classe */

/*!
	\class Window
	\author LEGOUT Paul
	\date 17.01.2020
	\brief Classe représentant la deuxième fenêtre. Hérite de QWidget; c'est un QWidget
*/
class Window: public QWidget
{
    Q_OBJECT

    				/* Définition des méthodes */

public:
    /*! 
		\param int masseMax, la masse maximale supportée par le paquebot renseignée par l'utilisateur précédemment
		\param int nombrePorts, le nombre de ports par lequel va passer le paquebot renseigné par l'utilisateur précédemment
		\param int nombreConteneurLargeur, le nombre de conteneurs que l'on peut placer en largeur sur le paquebot renseigné par l'utilisateur précédemment
		\param int nombreConteneurLongueur, le nombre de conteneurs que l'on peut placer en longueur sur le paquebot renseigné par l'utilisateur précédemment
		\param int nombreTotalConteneurs, le nombre total de conteneurs à placer sur le paquebot renseigné par l'utilisateur précédemment
		\brief Constructeur de la classe Window
    */
    Window(int masseMax, int nombrePorts, int nombreConteneurLargeur, int nombreConteneurLongueur,
           int nombreTotalConteneurs);
    
    /*!
		\date 17.01.2020
		\brief Genere une liste de conteneurs de masse et de numéro de port aléatoires (en fonction des données renseignées par l'utilisateur). En utilisation "réelle", cette fonction n'a pas lieu d'être car les différents conteneurs sont renseignés par la compagnie et non générés aléatoirement.
		\return Retourne la liste de conteneurs. C'est une liste de liste. La liste contient des listes correspondants aux informations (masse, port) des conteneurs pour chaque étage. 
		Liste générée sous la forme:
		[ConteneursEtageI] = [[Conteneur1], ....., [ConteneurN],...]
		où [ConteneurN] = [masseN,PortN, idConteneur]
    */
    QList<QList<float>> genereListeConteneur();

    /*!
        \date 17.01.2020
        \brief Vérifie dans la liste des conteneurs si l'identifiant existe
        \param int idConteneur, l'identifiant d'un conteneur
        \return true si l'identifiant est unique, false sinon
    */
    bool verifIdConteneur(int idConteneur);

    /*!
		\date 17.01.2020
		\brief Cette méthode tri la liste de conteneurs suivant le tri par insertion. Elle effectue un double tri: par numéros de port décroissants puis, par destination, par masses décroissantes. 
		Par masse décroissante car les conteneurs les plus lourds sont placés le plus proche possible du centre de gravité du paquebot.
		Par numéro de port décroissant car les conteneurs qui vont être déposés au premier port doivent se situer sur l'étage du haut afin de faciliter la décharge.
    */
    void triInsert();

    /*!
		\date 20.01.2020
		\brief Attribue à chaque conteneur des coordonnées sur le paquebot.
		Le Placement des conteneurs se fait "en escargot" ou "en serpentin":
		Le premier conteneur (dont la masse est la plus lourde et dont la destination est le dernier port par lequel s'arretera le paquebot) est placé au centre. Les conteneurs suivants se placent sur la première couronne autour du premier conteneur. Une fois la première couronne remplie, les conteneurs se placent sur la deuxième couronne. On effectue ceci jusqu'à ce que les conteneurs placés prennent toutes la largeur (si le nombre de conteneurs à placer en largeur renseigné est impair). Si le nombre de conteneurs à placer est pair, on décale le premier conteneur d'un rang sur la droite et effectue le remplissage en serpentin. Alors, la premiere colonne (la plus à gauche) est vide. on la remplit. 
		Enfin, on remplit, par ligne, afin de remplir totalement la longueur. 
		Ceci est effectué pour chaque étage.
    */
    void remplissagePaquebot();



    				
    				/* Définition des slots */

public slots:

	/*!
		\date 20.01.2020
		\brief Complément de la méthode "remplissagePaquebot". Ce slot permet d'appeler le slot "poseConteneur" toutes les 100ms.
	*/
    void simulation();
    
    /*!
		\date 20.01.2020
		\brief Pose un conteneur de la liste des conteneurs sur le paquebot selon ses coordonnées dans la liste des coordonnées générée par la méthode "remplissagePaquebot(). Ce slot est appelé pour chaque conteneur à placer. Création d'un label de couleur représentant le conteur et ajout un ToolTip permettant l'affichage des informations (masse et port) du conteneur lors du passage de la souris sur celui-ci.
    */
    void poseConteneur();


    				/* Définition des attributs */
private:

    	/* Définition des widgets */

    QPushButton *m_boutonCommencerSimultation;

    QHBoxLayout *m_hlayout;

    QGridLayout *m_glayout1;    //Grille 1er étage de conteneurs
    QGridLayout *m_glayout2;    //Grille 2ème étage de conteneurs
    QGridLayout *m_glayout3;    //Grille 3ème étage de conteneurs

    QVBoxLayout *m_vlayout;

    /* Barre de progression représentant l'avancement du remplissage du porte conteneurs */
    QProgressBar *m_barre;

    QTimer *m_timer;

    /* 3 fenêtres représentant graphiquement les 3 étages de conteneurs */
    QWidget *etage1;
    QWidget *etage2;
    QWidget *etage3;


    	/* Définition des autres attributs */
    
    int *m_masseMax;
    int *m_conteneursplaces;
    int *indice;

    const int *m_nombrePorts;
    const int *m_nombreConteneurLargeur;
    const int *m_nombreConteneurLongueur;
    const int *m_totalNombreConteneurs;

    float *m_masseTotale;


    /* Liste des conteneurs. C'est une liste de liste car chaque élément contient la masse du conteneur, son port de destination et l'identifiant du conteneur 
    Liste générée sous la forme:
        [ConteneursEtageI] = [[Conteneur1], ....., [ConteneurN],...]
        où [ConteneurN] = [masseN,PortN, idConteneur] */
    QList<QList<float>> *m_listeConteneurs;

	/* 
        Tableau 3 dimensions contenant les coordonnées de chaque conteneurs 
        Liste générée sous la forme:
            [[ConteneursEtage1], [ConteneursEtage2], [ConteneursEtage3]]
            où [ConteneursEtageI] = [[Conteneur1], ....., [ConteneurN],...]
            où [ConteneurN] = [x,y]
    */
    QList<QList<QList<int>>> *listeCoordonnees;

};

#endif // WINDOW_H
