/*!
    \file FirestWindow.cpp
    \author LEGOUT Paul
    \date 03.01.2020
    \brief Implémentation de la classe FirstWindow. 
*/

/* Inclusion du fichier d'entête */
#include "FirstWindow.h"


                    /* Implémentation du constructeur de la classe */

FirstWindow::FirstWindow():QWidget()
{

    /* Modification du titre de la fenêtre */
    this->setWindowTitle("Initialisation caractéristiques du paquebot");
    /* Initialisation du layout principal vertical de la fenêtre */
    QVBoxLayout *layout = new QVBoxLayout(this);        

    				
    				/* Initialisation des widgets */

    	/* Masse label et lineEdit */
    m_masseMaxLabel = new QLabel("Masse maximale supportée par le paquebot : (en tonnes)");
    m_masseMaxEdit = new QLineEdit;
    /* int validator afin d'être sûr que l'utilisateur saisisse une masse entière (en tonne) */
    QIntValidator *validatorMasse = new QIntValidator;
    m_masseMaxEdit->setValidator(validatorMasse);

    	/* Port label et lineEdit */
    m_nombrePortsLabel = new QLabel("Entrer le nombre de ports par lesquels le paquebot va passer");
    m_nombrePortsEdit = new QLineEdit;
    /* Ajout d'un int validator compris entre 1 et 50  */
    QIntValidator *validator = new QIntValidator(1,50);
    m_nombrePortsEdit->setValidator(validator);

    	/* Nombre de conteneurs que l'on peut placer en largeur sur le conteneur: label et lineEdit (maximum arbitrairement fixé à 30 grâce au QIntValidator afin de bien voir par la suite le placement des conteneurs sur l'ecran) */
    m_nombreConteneurLargeurLabel = new QLabel("Nombre de conteneurs que l'on peut placer en largeur");
    m_nombreConteneurLargeurEdit = new QLineEdit;
    QIntValidator *validatorLarg = new QIntValidator(1,30);
    m_nombreConteneurLargeurEdit->setValidator(validatorLarg);

    	/* Nombre de conteneur en longueur label et lineEdit */
    m_nombreConteneurLongueurLabel = new QLabel("Nombre de conteneurs que l'on peut placer en longueur");
    m_nombreConteneurLongeurEdit = new QLineEdit;
    QIntValidator *validatorLong = new QIntValidator(1,60);
    m_nombreConteneurLongeurEdit->setValidator(validatorLong);

    	/* Nombre de conteneurs total à placer sur le paquebot label et lineEdit. Ce nombre est forcément inférieur au nombre maximum total de conteneurs que l'on peut placer sur le paquebot, soit le nombre de conteneurs en largeur * le nombre de conteneurs en longueur *3. Le facteur 3 intervient ici car j'ai fixé arbitrairement le nombre d'étage de conteneurs à 3 */    
    m_totalNombreConteneursLabel = new QLabel("Nombre de conteneurs total à placer");
    m_totalNombreConteneursEdit = new QLineEdit;

    	/* Initialisation du bouton permettant de valider les informations saisies et passer à la suite */
    QPushButton *boutonValiderInitialisation = new QPushButton("Valider ces informations");

    
    	/* Ajout des différents widgets aux layouts qui définissent la structure de la fenêtre */
    layout->addWidget(m_masseMaxLabel);
    layout->addWidget(m_masseMaxEdit);
    layout->addWidget(m_nombrePortsLabel);
    layout->addWidget(m_nombrePortsEdit);
    layout->addWidget(m_nombreConteneurLargeurLabel);
    layout->addWidget(m_nombreConteneurLargeurEdit);
    layout->addWidget(m_nombreConteneurLongueurLabel);
    layout->addWidget(m_nombreConteneurLongeurEdit);
    layout->addWidget(m_totalNombreConteneursLabel);
    layout->addWidget(m_totalNombreConteneursEdit);
    layout->addWidget(boutonValiderInitialisation);


    
    	/* Connexion du bouton "Valider" au slot correspondant */
    connect(boutonValiderInitialisation, SIGNAL(clicked()), this, SLOT(validation()));


}

                   
                    /* Implémentation des méthodes de la classe */


/*  
	Slot appelé lors du clic sur le bouton valider
	Si l'utilisateur a appuyé sur le bouton valider et a rempli tous les champs correctement, alors on ouvre la seconde fenêtre
*/
void FirstWindow::validation()
{

		/* Si l'un des champs n'est pas rempli, renvoi d'un message d'erreur et annule la validation de la saisie */
    if (m_masseMaxEdit->text().isEmpty() || m_nombrePortsEdit->text().isEmpty() || m_nombreConteneurLargeurEdit->text().isEmpty()
            || m_nombreConteneurLongeurEdit->text().isEmpty() || m_totalNombreConteneursEdit->text().isEmpty())
    {
        QMessageBox::warning(0, "Erreur", "Vous devez entrer toutes les informations pour pouvoir continuer");
        return;
    }



    	/* Si le nombre de conteneur à placer est supérieur au nombre de conteneurs maximal supporté par le paquebot, soit le nombre de conteneurs en largeur * le nombre de conteneurs en longueur *3. Le facteur 3 intervient ici car j'ai fixé arbitrairement le nombre d'étage de conteneur à 3, renvoi d'un message d'erreur et annule la validation de la saisie */
    if(m_totalNombreConteneursEdit->text().toInt() > (m_nombreConteneurLargeurEdit->text().toInt()* m_nombreConteneurLongeurEdit->text().toInt())*3)
    {

        QString str ="Vous devez rentrer un nombre total de conteneurs inférieur ou égal à "
                + QString::number((m_nombreConteneurLargeurEdit->text().toInt()* m_nombreConteneurLongeurEdit->text().toInt())*3);
        QMessageBox::warning(0, "Erreur, trop de conteneurs",   str );
        m_totalNombreConteneursEdit->setText(QString::number((m_nombreConteneurLargeurEdit->text().toInt()* m_nombreConteneurLongeurEdit->text().toInt())*3));
        return;
    }


    
    	/* Si la masse maximale  supportée par le paquebot renseignée par l'utilisateur est trop faible par rapport au nombre de conteneur à placer retourne un message d'erreur et enlève les conteneurs en trop */
    if(m_masseMaxEdit->text().toFloat() < 1.5* m_totalNombreConteneursEdit->text().toInt())
    {
        /* Calcul du nombre de conteneurs à enlever par rapport à la saisie de l'utilisateur */
        float tmp = m_totalNombreConteneursEdit->text().toFloat();
        while (m_masseMaxEdit->text().toFloat() < 1.5*tmp)
            tmp --;
        
        /* Affichage du message d'erreur */
        QString str = "La masse totale des conteneurs est supérieure à la masse supportée par le paquebot. Il faut enlever des conteneurs. La masse maximale supportée par le paquebot permet de placer " + QString::number(tmp) + " conteneurs";
        QMessageBox::warning(0, "Erreur, masse maximale trop faible", str);

        /* Affectation du nouveau nombre de conteneur à placer sur le paquebot */
        m_totalNombreConteneursEdit->setText(QString::number(tmp));
        return;
    }


        /* Si toutes les informations saisies sont correctes, ouvre la deuxième fenêtre et masque la première */
    Window *window = new Window(m_masseMaxEdit->text().toInt(), m_nombrePortsEdit->text().toInt(),
                                m_nombreConteneurLargeurEdit->text().toInt(), m_nombreConteneurLongeurEdit->text().toInt(), m_totalNombreConteneursEdit->text().toInt());
    window->show();
    this->hide();

}


/* Méthode permettant l'appui sur la touche "Entrer" afin de valider les informations saisies */
void FirstWindow::keyPressEvent(QKeyEvent *event)
{

    if (event->key() == 16777220)
        validation();
}




