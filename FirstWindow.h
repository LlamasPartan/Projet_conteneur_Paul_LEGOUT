#ifndef FIRSTWINDOW_H
#define FIRSTWINDOW_H


/*!
    \file FirstWindow.h
    \author LEGOUT Paul
    \date 03.01.2020
    \brief Initialisation de la classe FirstWindow. Ouvre une fenêtre dans laquelle l'utilisateur doit renseigner des informations sur le porte-conteneurs (masse totale supportée, le nombre de ports par lequel il va passer et le nombre de conteneurs en largeur et longueur) et le nombre de conteneurs à transporter
*/

                    /* Inclusion des librairies */

/* Inclusion des widgets */
#include <QWidget>
/* Inclusion des layouts verticaux */
#include <QVBoxLayout>
/* Inclusion des boutons */
#include <QPushButton>
/* Inclusion des validateurs de saisie d'entier */
#include <QIntValidator>
/* Inclusion des labels */
#include <QLabel>
/* Inclusion des lignes de saisie de texte */
#include <QLineEdit>
/* Inclusion de la seconde fenêtre */
#include <Window.h>
/* Inclusion des flux entrée/sortie */
#include <iostream>
/* Inclusion des boites de dialogue affichant un message */
#include <QMessageBox>
/* Inclusion des évènements claviers  */
#include <QKeyEvent>

using namespace std;


                    /* Définition de la classe */

/*!
    \class FirstWindow
    \author LEGOUT Paul
    \date 03.01.2020
    \brief Classe représentant la première fenêtre. Hérite de QWidget; c'est un QWidget.
*/
class FirstWindow:public QWidget
{
    Q_OBJECT

                    
                    /* Définition des méthodes */
public:
    
    /*!
        \brief Constructeur de la classe FirstWindow
    */
    FirstWindow();
    /*!
        \brief Méthode qui permet de valider la saisie grâce à la touche entrée
    */
    virtual void keyPressEvent(QKeyEvent *event);


                    
                    /* Définition des slots */
public slots:
    
    /*!
        \brief Slot permettant l'ouverture de la seconde fenêtre. Il est connecté à un bouton.
    */
    void validation();

                   
                    /* Définition des attributs */
private:

        /* Définition des widgets */
    QLabel *m_masseMaxLabel;
    QLineEdit *m_masseMaxEdit;
    QLabel *m_nombrePortsLabel;
    QLineEdit *m_nombrePortsEdit;
    QLabel *m_nombreConteneurLargeurLabel;
    QLineEdit *m_nombreConteneurLargeurEdit;
    QLabel *m_nombreConteneurLongueurLabel;
    QLineEdit *m_nombreConteneurLongeurEdit;
    QLabel *m_totalNombreConteneursLabel;
    QLineEdit *m_totalNombreConteneursEdit;

        /* Définition des autres attributs */
    int *m_nmbreConteneurLargeur;
    int *m_nmbreConteneurLongueur;
};

#endif // FIRSTWINDOW_H
