/*!
    \file Window.cpp
    \author LEGOUT Paul
    \date 17.01.2020
    \brief Implémentation de la classe Window
*/

/* Inclusion du fichier d'entête */
#include "Window.h"

                    
                    /* Implémentation du constructeur de la classe */

Window::Window(int masseMax, int nombrePorts, int nombreConteneurLargeur, int nombreConteneurLongueur,int nombreTotalConteneurs):QWidget ()
{

    /* Modification du titre de la fenêtre */
    this->setWindowTitle("Automatisation remplissage paquebot");


            /* Initialisation des attributs */

    m_masseMax = new int(masseMax);
    m_masseTotale = new float(0);
    m_nombrePorts = new int(nombrePorts);
    m_nombreConteneurLargeur = new int(nombreConteneurLargeur);
    m_nombreConteneurLongueur = new int(nombreConteneurLongueur);
    m_totalNombreConteneurs = new int(nombreTotalConteneurs);
    m_conteneursplaces = new int(0);
    m_listeConteneurs = new QList<QList<float>>;
    
    /* Affectation de la liste de conteneurs */
    *m_listeConteneurs +=genereListeConteneur();        
    
    /* Tri de la liste de conteneurs: par destinations décroissantes puis, par destination, par masses décroissantes */
    triInsert();


    indice = new int(0);    /* Variable comptant le nombre de conteneurs créés graphiquement */


    /* 
        Liste de liste de liste qui contiendra les coordonnées de chaque conteneur
        Liste générée sous la forme:
            [[ConteneursEtage1], [ConteneursEtage2], [ConteneursEtage3]]
            où [ConteneursEtageI] = [[Conteneur1], ....., [ConteneurN],...]
            où [ConteneurN] = [masseN,PortN] 
    */
    listeCoordonnees = new QList<QList<QList<int>>> ;


            /* Initialisation des widgets */

    m_boutonCommencerSimultation = new QPushButton("Commencer simulation");

    /* Initialisation de la barre de progression:
       Intervalle de valeurs : {0, ...., 100}
       Valeur initialisée à 0
    */
    m_barre = new QProgressBar;
    m_barre->setRange(0,100);
    m_barre->setValue(0);


        /* Initialisation des 3 fenêtres qui représenteront chacune un étage de conteneurs */

    etage1 = new QWidget;
    etage1->setWindowTitle("Premier étage de conteneurs");
    etage1->setMinimumSize(nombreConteneurLargeur*50, nombreConteneurLongueur*80);

    etage2 = new QWidget;
    etage2->setWindowTitle("Deuxième étage de conteneurs");
    etage2->setMinimumSize(nombreConteneurLargeur*50, nombreConteneurLongueur*80);

    etage3 = new QWidget;
    etage3->setWindowTitle("Troisème et dernier étage de conteneurs");
    etage3->setMinimumSize(nombreConteneurLargeur*50, nombreConteneurLongueur*80);



        /* Initialisation des layouts de la fenêtre*/

    /* Layout horizontal secondaire de la fenêtre */
    m_hlayout = new QHBoxLayout;

    /* Grid layout des 3 étages de conteneurs */
    m_glayout1 = new QGridLayout(etage1);
    m_glayout2 = new QGridLayout(etage2);
    m_glayout3 = new QGridLayout(etage3);

    /* Layout vertical principal de la fenêtre */
    m_vlayout = new QVBoxLayout(this);      



    
        /* Initialisation des labels de la fenêtre qui rappeleront les informations sur le porte conteneurs */

    /* Masse maximale supportée par le paquebot */
    QLabel* masse = new QLabel;
    masse->setText("La masse maximale supportée par le paquebot est : " + QString::number(masseMax) + " tonnes\n");

    /* Masse totale des conteneurs à placer sur le paquebot */
    QLabel *masseCont = new QLabel;
    masseCont->setText("La masse totale de conteneurs sur le paquebot est : " + QString::number(*m_masseTotale) + " tonnes\n");

    /* Le nombre de ports par lequel s'arretera le porte conteneurs */
    QLabel *ports = new QLabel;
    ports->setText("Le porte conteneurs passe par " + QString::number(nombrePorts) + " ports\n" );

    /* Le nombre de conteneurs que l'on peut placer en largeur sur le paquebot */
    QLabel *conteneurLargeur = new QLabel;
    conteneurLargeur->setText("On peut placer " + QString::number(nombreConteneurLargeur) + " conteneurs en largeur\n");

    /* Le nombre de conteneurs que l'on peut placer en longueur sur le paqubebot */
    QLabel *conteneurLongueur = new QLabel;
    conteneurLongueur->setText("On peut placer " + QString::number(nombreConteneurLongueur) + " conteneurs en longueur\n");

    /* Le nombre de conteneurs total que l'on peut placer sur le paquebot (nombre de conteneurs que l'on peut placer en largeur * nombre de conteneurs que l'on peut placer en longueur * les 3 étages) */
    QLabel *conteneurTotal = new QLabel;
    conteneurTotal->setText("Le nombre de conteneurs total que l'on peut placer sur le porte conteneurs est : "
                            + QString::number(nombreConteneurLargeur*nombreConteneurLongueur*3) + "\n");
    
    /* Le nombre de conteneur à placer sur le paquebot */
    QLabel *conteneur = new QLabel;
    conteneur->setText("Le nombre de conteneurs à placer est : " + QString::number(nombreTotalConteneurs) + "\n");


        /* Ajout des widgets aux layouts */

    m_hlayout->addWidget(m_barre);
    m_hlayout->addWidget(m_boutonCommencerSimultation);

    m_vlayout->addWidget(masse);
    m_vlayout->addWidget(masseCont);
    m_vlayout->addWidget(ports);
    m_vlayout->addWidget(conteneurLargeur);
    m_vlayout->addWidget(conteneurLongueur);
    m_vlayout->addWidget(conteneurTotal);
    m_vlayout->addWidget(conteneur);


    /* Ajout du layout secondaire (horizontal) au layout principal (vertical) */
    m_vlayout->addLayout(m_hlayout);


    /* Execution de la méthode "remplissagePaquebot()" afin d'affecter chaque conteneur à des coordonnées sur le paquebot */
    remplissagePaquebot();


    /* Définition du Timer qui sera initialisé dans le slot "simulation()". Ce Timer et ce slot permettront l'appel du slot "poseConteneur()" toutes les 100ms afin de placer chaque conteneur, toutes les 100ms, aux coordonnnées affectées précédemment grâce à la méthode "remplissagePaquebot()" */
    m_timer = new QTimer(this);


    /* Pour finir, connexion du bouton "Commencer Simulation" au slot "simulation()" afin d'initialiser le Timer et commencer le placement des conteneurs sur le paquebot */
    connect(m_boutonCommencerSimultation, SIGNAL(clicked()), this, SLOT(simulation()));

}


                    /* Implémentation des méthodes de la classe */


/* 
    Génère la liste de conteneurs à placer sur le paquebot
    Liste générée sous la forme:
        Liste générée sous la forme:
        [[Conteneur1], ....., [ConteneurN],...]
        où [ConteneurN] = [masseN,PortN, idConteneur]
*/
QList<QList<float>> Window::genereListeConteneur()
{

    /* Définition de la liste de liste de réels*/
    QList<QList<float>> listeConteneurs;
    
        /* Tant que le nombre de conteneurs créés est inférieur au nombre de conteurs à créer (le nombre de conteneurs à placer renseigné par l'utilisateur), on créer un nouveau conteneur et l'ajoute à la liste */
    int i(0);
    while (i<*m_totalNombreConteneurs && floor(*m_masseTotale +2.1) < *m_masseMax)
    {
        /* Création d'une liste de réels pour chaque conteneur qui contiendra 2 éléments: la masse du conteneur et son numéro de port de destination */
        QList<float> listeInformationConteneurs;
        
        /* Génération d'une masse aléatoire entre 1 500kg et 2 100kg (intervalle de masses moyennes d'un conteneur) */
        int masse =  rand()%600 + 1500;       
        
        /* Ajout de cette masse, convertie en tonne, à la liste de réels en première position*/
        listeInformationConteneurs.append((float)masse/1000); 

        /* Génération d'un numéro de port aléatoire compris entre 1 et le nombre de ports renseigné par l'utilisateur */
        int destination = rand()%*m_nombrePorts +1;   

        /* Ajout  de ce numéro de port à la liste de réels en deuxième position */
        listeInformationConteneurs.append(destination);

        	/* Génération d'un numéro de conteneur à 5 chiffres - entre 10 000 et 99 999 
        	Cet identifiant est unique, on vérifie donc qu'aucun des précédents conteneurs n'a cet identifiant*/
        int tmp = rand()%90000 + 10000;
        int idConteneur;
        while (!verifIdConteneur(tmp))
        	tmp = rand()%90000 + 10000;

        /* Identifiant unique, on affecte donc cet id à sa variable */
        idConteneur = tmp;

        /*  Ajout de l'id du conteneur à la liste de réels en troisième position */
        listeInformationConteneurs.append(idConteneur);

        /* Ajout de la liste de réels à la liste de liste contenant les informations de tous les conteneurs */
        listeConteneurs.append(listeInformationConteneurs);
        
        i++;
        *m_masseTotale += masse/1000;
    }

    /* Quand le liste a été totalement générée, retourne cette liste de liste */
    return listeConteneurs;
}

/* Cette méthode permet de vérifier si l'identifiant généré a déjà été attribué à un conteneur */
bool Window::verifIdConteneur(int idConteneur)
{
	/* Boucle parcourant l'ensemble des identifiants déjà attribués */
	for (int i(0); i< m_listeConteneurs->size(); i++)
	{
		/* Comparaison de l'identifiant du ième conteneur avec celui généré */
		if (idConteneur == m_listeConteneurs[0][i][2])
			return false;
	}

	return true;
}

/* Tri par insertion de la liste générée par numéro de port de destination décroissant puis, pour une même destination, par masse décroissante */
void Window::triInsert()
{

        /* Tri par numéro de port de destination décroissant */

    for (int i(1) ; i < m_listeConteneurs->size(); i++)
    {
        /* Pour chaque élément de la liste de liste des conteneurs, le port de destination est en deuxième position */
        float pivot = m_listeConteneurs[0][i][1];
        QList<float> temp = m_listeConteneurs[0][i];
        int j = i-1;
        while (j >= 0 && pivot > m_listeConteneurs[0][j][1])
        {
            m_listeConteneurs[0][j+1] = m_listeConteneurs[0][j];
            j --;
        }
        m_listeConteneurs[0][j+1] = temp;
    }


        /* Tri par masse décroissante pour chaque numéro de port */

    for (int i(1); i< m_listeConteneurs->size(); i++)
    {
        /* Pour chaque élément de la liste de liste des conteneurs, la masse est en première position */
        float pivot = m_listeConteneurs[0][i][0];
        QList<float> temp = m_listeConteneurs[0][i];
        int j = i-1;
        while ( j >= 0  && m_listeConteneurs[0][j][1] == m_listeConteneurs[0][i][1] && pivot > m_listeConteneurs[0][j][0])
        {
            m_listeConteneurs[0][j+1] = m_listeConteneurs[0][j];
            j --;
        }
        m_listeConteneurs[0][j+1] = temp;
    }
}


/* Affectation de coordonnées à chaque conteneur */
void Window::remplissagePaquebot()
{

    /* Soit x0 la ligne du centre de gravité et y0 sa colonne (l'axe des x descendant et l'axe des y allant vers la droite) */
    int x0 = floor(*m_nombreConteneurLongueur/2)  ;
    int y0 = floor(*m_nombreConteneurLargeur/2)  ;
    
    /* Variable comptant le nombre de conteneurs dont les coordonées ont été affectés */
    int nmbreConteneursPlaces = 0;

        
        /* Affectation des coordonnées "en serpentin", "en escargot" pour chaque conteneur et pour chaque étage */

    /* Pour chaque etage on effectue le remplissage de conteneurs */
    for (int etage(0); etage < 3; etage ++)
    {
        /* Variable comptant le nombre de conteneurs placés par étage. Il faut que ce nombre ne dépasse pas le nombre de conteneurs que l'ont peut placer par etage (nombre de conteneurs que l'on peut placer en largeur * le nombre de conteneurs que l'on peut placer en longueur) */
        int nmbreConteneursEtage =0;

        /* Liste de liste qui contiendra toutes les coordonnées [x,y] des conteneurs d'un étage */
        QList<QList<int>> *coordonnees = new QList<QList<int>>;

            
                /* Remplissage "en escargot" */
        
        QList<int> *grav = new QList<int> ;
        
            /* On place le premier conteneur de coordonnées le centre de gravité et on l'ajoute à la liste des coordonnées de conteneurs */
        if (nmbreConteneursPlaces < *m_totalNombreConteneurs)
        {
            
            grav->append(x0);
            grav->append(y0);

            coordonnees->append(*grav );
            nmbreConteneursPlaces ++;
            nmbreConteneursEtage ++;
        }

        /* Indice indiquant la couronne de remplissage */
        int k =1;       

            /* Remplissage du carré autour du centre de gravité, "en escargot", par couronne autour du premier conteneur.
            Tant que le nombre de conteneurs placés est inférieur au nombre total de conteneurs à placer sur le paquebot et tant que le nombre de conteneurs placés en largeur via cette méthode est inférieure au nombre de conteneur que l'on peut placer en largeur, remplissage */
        do
        {
            /* Remplissage de la colonne de gauche, de haut en bas*/
            for (int i(0); i < 2*k; i++)
            {
                if (nmbreConteneursPlaces < *m_totalNombreConteneurs
                        && nmbreConteneursEtage < *m_nombreConteneurLargeur * *m_nombreConteneurLongueur)
                {
                    QList<int> *point = new QList<int>;
                    point->append(x0 -k+1 + i);
                    point->append(y0 - k);
                    coordonnees->append(*point);
                    nmbreConteneursPlaces++;
                    nmbreConteneursEtage ++;
                }

            }
            /* Remplissage de la ligne du dessous, de gauche à droite*/
            for (int i(0); i < 2*k ; i++)
            {
                if (nmbreConteneursPlaces < *m_totalNombreConteneurs
                        && nmbreConteneursEtage < *m_nombreConteneurLargeur * *m_nombreConteneurLongueur )
                {
                    QList<int> *point = new QList<int>;
                    point->append(x0+k);
                    point->append(y0-k+1 + i);
                    coordonnees->append(*point);
                    nmbreConteneursPlaces++;
                    nmbreConteneursEtage ++;
                }

            }
            /* Remplissage de la colonne de droite, de bas en haut */
            for (int i(0); i<2*k; i++)
            {
               if (nmbreConteneursPlaces < *m_totalNombreConteneurs
                       && nmbreConteneursEtage < *m_nombreConteneurLargeur * *m_nombreConteneurLongueur)
               {
                   QList<int> *point = new QList<int>;
                   point->append(x0+k-i-1);
                   point->append(y0 +k);
                   coordonnees->append(*point);
                   nmbreConteneursPlaces++;
                   nmbreConteneursEtage ++;
               }

            }
            /* Remplissage de la ligne du dessus, de gauche à droite */
            for (int i(0); i<2*k; i++)
            {
                if (nmbreConteneursPlaces < *m_totalNombreConteneurs
                        && nmbreConteneursEtage < *m_nombreConteneurLargeur * *m_nombreConteneurLongueur )
                {
                    QList<int> *point = new QList<int>;
                    point->append(x0-k);
                    point->append(y0+k-i-1);
                    coordonnees->append(*point);
                    nmbreConteneursPlaces++;
                    nmbreConteneursEtage ++;
                }


            }

            /* La couronne k est remplie, on passe à la couronne k+1 */
            k++;

        }while (k <= floor((*m_nombreConteneurLargeur/2)
                           -(*m_nombreConteneurLargeur-1)%2) && nmbreConteneursPlaces < *m_totalNombreConteneurs );


        /* Si le nombre de conteneur que l'on peut placer en largeur est pair, on a décalé le premier conteneur, placé au centre, d'un rang sur la droite. Alors la méthode de remplissage en escargot n'a pas rempli la colonne la plus à gauche. On la remplit alors */
        if (*m_nombreConteneurLargeur%2 == 0)
        {
            for (int j(0); j < 2*k-1; j++)
            {
                if (nmbreConteneursPlaces < *m_totalNombreConteneurs
                        && nmbreConteneursEtage < *m_nombreConteneurLargeur * *m_nombreConteneurLongueur)
                {
                    QList<int> *point = new QList<int>;
                    point->append(x0-k+1+j);
                    point->append(0);
                    coordonnees->append(*point);
                    nmbreConteneursPlaces++;
                    nmbreConteneursEtage ++;
                }

            }
        }

            /* Jusqu'ici, on a rempli un carré de coté le nombre de conteneur en largeur/2 (et eventuellement la première colonne de gauche). Il faut donc s'occuper des conteneurs à placer sur le reste de la longueur, c'est à dire en haut et en bas du carré déjà rempli. On effectue ce remplissage par ligne en alternant ligne en haut et ligne en bas, toujours dans un soucis d'equilibrer au maximum la charge sur le paquebot */
        
        /* Indice permettant l'alternance une fois sur deux ligne du haut, ligne du bas. Si (-1)^i est positif (=1) alors on remplit la ligne du bas, sinon on remplit la ligne du haut */
        int i = 0;  
        do{
            /* Remplissage ligne du bas */
            if (pow(-1,i)>0)
            {
                for (int j(0); j < *m_nombreConteneurLargeur; j++)
                {
                    if (nmbreConteneursPlaces < *m_totalNombreConteneurs
                            && nmbreConteneursEtage < *m_nombreConteneurLargeur * *m_nombreConteneurLongueur)
                    {
                        QList<int> *point = new QList<int>;
                        point->append(x0-k);
                        point->append(j);
                        coordonnees->append(*point);
                        nmbreConteneursPlaces++;
                        nmbreConteneursEtage ++;
                    }


                }
            }
            /* Remplissage ligne du haut */
            if(pow(-1,i) <0)
            {
                for (int j(0); j < *m_nombreConteneurLargeur; j++)
                {
                    if (nmbreConteneursPlaces < *m_totalNombreConteneurs
                            && nmbreConteneursEtage < *m_nombreConteneurLargeur * *m_nombreConteneurLongueur)
                    {
                        QList<int> *point = new QList<int>;
                        point->append(x0+k);
                        point->append(j);
                        coordonnees->append(*point);
                        nmbreConteneursPlaces++;
                        nmbreConteneursEtage ++;

                    }


                }
                k++;
            }

            i++;

        }while(nmbreConteneursPlaces < *m_totalNombreConteneurs
               && nmbreConteneursEtage < *m_nombreConteneurLargeur* *m_nombreConteneurLongueur);



        /* L'étage est rempli, on ajoute la liste des coordonnées des conteneurs pour un etage à la liste contenenant les coordonnées de tous conteneurs
        listeCoordonnees de la forme :
            [[ConteneursEtage1], [ConteneursEtage2], [ConteneursEtage3]]
            où [ConteneursEtageI] = [[Conteneur1], ....., [ConteneurN],...]
            où [ConteneurN] = [masseN,PortN, idConteneur]  */
        listeCoordonnees->append(*coordonnees);
    }

}

/* Initialisation et connexion du Timer défini dans le constructeur au slot "poseConteneur()". Ce slot sera appelé toutes les 100ms afin de poser un conteneur */
void Window::simulation()
{
    connect(m_timer, SIGNAL(timeout()), this, SLOT(poseConteneur()));
    m_timer->start(100);
}


/* 
    Pose du conteneur de la liste des conteneurs sur le paquebot selon ses coordonnées dans la liste   des coordonnées générée par la méthode "remplissagePaquebot()".
    Chaque conteneur est représenté graphiquement par un label de couleur. 

  */
void Window::poseConteneur()
{
    if (*indice < *m_totalNombreConteneurs )
    {
            /* Création du label de couleur (rouge pour ceux du premier étage, vert pour ceux du deuxième et bleu pour ceux du troisième) */
        QLabel *label = new QLabel;
        label->setFixedSize(30,60);
        label->setCursor(Qt::PointingHandCursor);
        /* Ajout du ToolTip permettant l'affichage des infomations du conteneur lors du passage de la souris sur celui-ci */
        QString *infos = new QString;
        *infos = "Masse: "+ QString::number(m_listeConteneurs[0][*indice][0])
                + " tonnes \nPort: " + QString::number(m_listeConteneurs[0][*indice][1])
                + "\nIdentifiant: " + QString::number(m_listeConteneurs[0][*indice][2]);
        label->setToolTip(*infos);
        
        /* Premier étage de conteneurs */
        if (*indice < *m_nombreConteneurLargeur * *m_nombreConteneurLongueur)
        {
            if (*indice == 0)
                etage1->show();

            label->setStyleSheet(QString::fromUtf8("background-color:rgb(255,0,0);"));  
            m_glayout1->addWidget(label, listeCoordonnees[0][0][*indice][0], listeCoordonnees[0][0][*indice][1]);

        }
       
        /* Deuxième étage de conteneurs */
        else if (*indice <(*m_nombreConteneurLargeur * *m_nombreConteneurLongueur)*2)
        {
            if (*indice == *m_nombreConteneurLargeur * *m_nombreConteneurLongueur)
                etage2->show();

            int j = *indice -*m_nombreConteneurLargeur * *m_nombreConteneurLongueur;
            label->setStyleSheet(QString::fromUtf8("background-color:rgb(0,255,0);"));  //Deuxième étage rempli de conteneurs verts
            m_glayout2->addWidget(label, listeCoordonnees[0][1][j][0], listeCoordonnees[0][1][j][1]);
        }
        
        /* Troisième et dernier étage de conteneurs */
        else
        {
            if (*indice == (*m_nombreConteneurLargeur * *m_nombreConteneurLongueur)*2 )
                etage3->show();

            int j = *indice -(*m_nombreConteneurLargeur * *m_nombreConteneurLongueur)*2;
            label->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,255);")); //Troisième étage rempli de conteneurs bleus
            m_glayout3->addWidget(label, listeCoordonnees[0][2][j][0], listeCoordonnees[0][2][j][1]);
        }
        *indice +=1;

    }

    /* Mise à jour de la barre de progression représentant l'avancement du remplissage */
    m_barre->setValue(floor(*indice * 100/ *m_totalNombreConteneurs));

    /* Une fois tous les conteneurs placés, affichage d'un message et suppression du Timer */
    if (*indice == *m_totalNombreConteneurs)
    {
        QMessageBox::information(this, "Fin de tâche", "Tous les conteneurs ont été placés avec succès!");
        m_timer->deleteLater();
    }

}


