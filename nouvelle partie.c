#include "header.h"

//Saisit des informations des joueurs avant de lancer une nouvelle partie
void nouvelle_partie()
{
    ///Déclaration + allocation dynamique
    int action = 0;
    int nb_joueur = 0;
    int validation = 0;
    int ordi=0;

    t_image *element;
    element = (t_image*)malloc(sizeof(t_image)*NB_ELEMENTB);

    //Creer le buffer, une zone sur laquelle sont affichées toutes les images
    BITMAP *buffer;
    buffer = create_bitmap(800, 600);

    ///Téléchargement des images utilisées dans cette partie du menu
    initialisation_image_nouvelle_partie(element);

    ///Tant que l'utilisateur n'a pas fait de choix on affiche le menu de choix du nombre de joueurs
    choix_nb_joueurs(element,buffer,&nb_joueur,&validation,&action);
    //1 joueur = 1 joueur + 1 ordi
    if(nb_joueur==1)
    {
        nb_joueur=2;
        ordi=1;
    }

    ///Si action == 2, cela signifie que l'utilisateur a choisit un nombre de joueur et souhaite passer à la suite
    if(action == 2)
    {
        clear_bitmap(screen); //On nettoie l'écran pour la suite
        ChoisirIconesNoms(nb_joueur, ordi);
        gif();
    }

    ///On libère le tableau d'image qui servait au menu nouvelle partie
    liberation_element(element,2);
}


//Sous programme permettant d'afficher le menu de sélection du nombre de joueurs, il retourne le nombre choisit par pointeur.
void choix_nb_joueurs(t_image *element,BITMAP *buffer, int *choix, int *validation,int *action)
{
    ///Déclaration
    int i, j = 2, appui=0;
    BITMAP *souris;                     //La souris
    souris = load_bitmap("Autres/souris.bmp", NULL);

    //Tant que l'utilisateur n'a pas fait de choix on affiche le menu de selection du nbre de joueur
    while(*action==0)
    {
        //J correspond au nombre de joueur, donc on le remet à 1 à chaque nouvelle boucle
        j=1;

        ///Met un fond sur le buffer
        blit(element[0].fichier, buffer, element[0].pos_x, element[0].pos_y, 0, 0, element[0].taille_x, element[0].taille_y);

        ///On affiche la case nombre de joueur
        draw_sprite(buffer, element[5].fichier, element[5].pos_x, element[5].pos_y);

        ///Sélection nombre de joueurs
        for(i=6; i<NB_ELEMENTB-1; i+=2)
        {
            ///Affichage case numéro j
            if((*choix == j)||(mouse_x>=element[i].pos_x && mouse_x<=element[i].taille_x && mouse_y>=element[i].pos_y && mouse_y<=element[i].taille_y))
            {
                //Affichage avec effet de sélection
                draw_sprite(buffer, element[i+1].fichier, element[i+1].pos_x, element[i+1].pos_y);

                //Si l'utilisateur a cliquer sur un chiffre, la variable choix prend cette valeur
                if(mouse_b&1)
                    *choix = j;

            }
            else
            {
                ///La souris n'est pas sur la case j
                draw_sprite(buffer, element[i].fichier, element[i].pos_x, element[i].pos_y);
            }

            j++;

        }

        ///Si l'utilisateur a cliqué sur suivant sans avoir choisit un nombre de joueur on affiche des bords rouge
        if(*validation==1)
            ///Affichage de la case nombre de joueur en rouge
            draw_sprite(buffer,element[22].fichier,element[22].pos_x,element[22].pos_y);

        ///La souris est sur la case suivant donc on l'affiche avec un effet de sélection
        if(mouse_x>=element[3].pos_x && mouse_x<=element[3].taille_x && mouse_y>=element[3].pos_y && mouse_y<=element[3].taille_y)
        {
            draw_sprite(buffer, element[4].fichier, element[4].pos_x, element[4].pos_y);

            if(mouse_b&1)
            {
                ///L'utilisateur a cliqué sur suivant sans choisir le nbre de joueur
                if(*choix==0)
                    *validation = 1;

                ///L'utilisateur a cliqué sur suivant après avoir choisit le nbre de joueur
                else
                    *action = 2;
            }
        }
        else
            ///La souris n'est pas sur la case suivant donc on l'affiche sans effet de sélection
            draw_sprite(buffer, element[3].fichier, element[3].pos_x, element[3].pos_y);

        ///La souris est sur la case retour donc on l'affiche avec un effet de sélection
        if(mouse_x>=element[1].pos_x && mouse_x<=element[1].taille_x && mouse_y>=element[1].pos_y && mouse_y<=element[1].taille_y)
        {
            draw_sprite(buffer, element[2].fichier, element[2].pos_x, element[2].pos_y);

            ///L'utilisateur a cliqué sur la case retour
            if(mouse_b&1)
                appui=1;
            if(!(mouse_b & 1) && appui==1)//Le joueur ne clique plus plus activer retour
                *action = 1;
        }
        else
        {
            ///La souris n'est pas sur la case retour donc on l'affiche sans effet de sélection
            draw_sprite(buffer, element[1].fichier, element[1].pos_x, element[1].pos_y);
            appui=0;
        }
        draw_sprite_h_flip(buffer, souris, mouse_x-2, mouse_y-20);  //Afficher souris

        //Met à jour le buffer (permet d'afficher toutes les images en même temps)= pas de clignotements
        blit(buffer, screen, 0, 0, 0, 0, element[0].taille_x, element[0].taille_y);
    }

    destroy_bitmap(souris);
}


//Sous programme qui gère la sélection des pseudos et logos des nouveaux joueurs
void ChoisirIconesNoms(int nbJoueurs, int ordi)
{
    int i, j, pepite[3], val; //Les 3 cartes pépites
    char defausse[10];
    sprintf(defausse, "0");
    BITMAP *plateau;    //L'image contenant le disposition des cartes du plateau
    plateau = create_bitmap(800, 600);
    t_joueur joueur[8];
    t_carte tab[70];
    t_carte tabmelangee[70];
    t_Pile * pile = (t_Pile*)malloc(sizeof(t_Pile));

    ///Init images
    t_image *nain;
    nain = (t_image*)malloc(sizeof(t_image)*8);
    initNains(nain);
    putpixel(plateau, 180-74/2, 165+84/2+2*84, makecol(0, 255, 0));//Ajoute la carte de départ au plateau
    putpixel(plateau, 180-1, 165+84/2+2*84, makecol(255, 0, 0));//Ajoute la carte de départ au plateau

    initJoueur(joueur, nain, nbJoueurs, ordi);

    ///Initialiser la pile
    initialiserJeu(tab);
    melangerJeu(tab,tabmelangee);
    creationPioche(pile,tabmelangee);

    ///Initialiser les pépites
    for(i=0; i<3; i++)
        pepite[i]=0;
    for(i=0; i<3; i++)
    {
        do
        {
            val = rand()% (3 - 1 +1)+1;
            if(val==pepite[0] || val==pepite[1] || val==pepite[2])//Recommencer l'attribution de la valeur
                val=1;
            else
            {
                pepite[i] = val;
                val=0;
            }
        }
        while(val);
    }

    ///Donner 6 cartes à chaque joueur:
    for(i=0; i<nbJoueurs; i++)
    {
        sprintf(joueur[i].cartes[6],"0");
        for(j=0; j<6; j++)
            strcpy(joueur[i].cartes[j], piocher(pile));   //Donne les cartes aux joueurs (char)
    }

    ///Jeu
    MainPlateau(nbJoueurs, nain, joueur, pile, plateau, pepite, defausse, ordi, 0); //Aff le plateau de chaque joueur

    destroy_bitmap(plateau);

}


//Choix des pseudos des personnages
void ChoixPseudo(t_joueur *joueur, BITMAP *cadre)   //Le joueur prend son nom
{
    int nbLettres=0, i, lettreInt;
    char nom[16], lettre;
    BITMAP *buffTemp;
    buffTemp = create_bitmap(180, 20);

    for(i=0; i<15; i++)
        nom[i]=0;

    do
    {
        if(keypressed())
        {
            lettreInt=0;
            lettreInt = readkey();
            lettre = lettreInt;
            if(lettre>='a' && lettre<='z' && nbLettres<15)
            {
                nom[nbLettres] = lettre;
                nbLettres++;
            }
            else if(lettre==8)
            {
                nom[nbLettres]=0;
                if(nbLettres>0)
                    nbLettres--;
            }
        }

        stretch_sprite(buffTemp, cadre, -50, -50, 219, 200);//cadre blanc
        for(i=0; i<nbLettres; i++)
        {
            textprintf_ex(buffTemp, font, 10+8*i, 5, makecol(0, 0, 0), -1, "%c", nom[i]); //Affiche le nom actuel
        }

        blit(buffTemp, screen, -630, -5, 0, 0, SCREEN_W, SCREEN_H);
    }
    while(!key[KEY_ENTER] || nom[0]==0);
    strcpy(joueur->pseudo,nom);

    destroy_bitmap(buffTemp);
}


///-----------------------INITIALISATION DES IMAGES-----------------------///
void initNains(t_image nain[8])
{
    int i;
    for(i=0; i<8; i++)
    {
        sprintf(nain[i].nomfichier, "Nains/nain %d.bmp", i+1);
        nain[i].fichier = load_bitmap(nain[i].nomfichier, NULL);
        nain[i].pos_x=30+90*i;
        nain[i].pos_y=400;
        nain[i].taille_x=80;
        nain[i].taille_y=145;

        if(!nain[i].fichier)
            allegro_message("Pb %s", nain[i].nomfichier);
    }
}

//initialise les variables des joueurs
void initJoueur(t_joueur tab[8], t_image nain[8], int nbJoueurs, int ordi)
{
    int i, j, k;
    int saboteur=0, saboteur2=0;
    char message[20];
    int points=0;

    t_image *sortie;
    sortie = (t_image*)malloc(sizeof(t_image)*2);
    initSortie(sortie);

    for(i=0; i<nbJoueurs; i++)
    {
        tab[i].outils[0]=1;
        tab[i].outils[1]=1;
        tab[i].outils[2]=1;
        tab[i].score=0;
        tab[i].pseudo[0] = 'N';
        tab[i].pseudo[1] = 'o';
        tab[i].pseudo[2] = 'm';
        sprintf(tab[i].role,"chercheur");
        for(j=3; j<16; j++)
            tab[i].pseudo[j]=0;

        if(i==1 && ordi==1)//Initialiser l'ordi
        {
            sprintf(tab[0].role,"chercheur");
            sprintf(tab[1].role,"saboteur");
            saboteur=1;
            sprintf(tab[i].pseudo,"ordinateur");
            for(j=0; j<8; j++)
            {
                tab[i].logo = j;
                for(k=0; k<nbJoueurs; k++)//Logo pris ou pas:
                {
                    if(i!=1 && tab[i].logo!=tab[k].logo)//Logo libre
                    {
                        k=nbJoueurs;
                        j=8;
                    }
                }
            }
        }
        else //Le joueur choisit son personnage:
            prendreLogo(tab, nain, i, sortie);

        ///Attribution des rôles saboteur
        if(saboteur==0)
        {
            saboteur = rand()%((nbJoueurs-1) - 0 + 1) + 0;
            do
            {
                if(nbJoueurs>7)
                    saboteur2 = rand()%((nbJoueurs-1) - 0 + 1) + 0;
            }
            while(saboteur2 == saboteur);
        }
        sprintf(tab[saboteur].role,"saboteur");
        if(nbJoueurs>7)
            sprintf(tab[saboteur2].role,"saboteur");

        sprintf(message, "       Vous etes ");
        strcat(message, tab[i].role);
        if(!(ordi==1 && i==1))
            afficherMessage(message);

        ///Reprendre son score précédent
        FILE* fichier = NULL;
        fichier = fopen("sauvegarde_score.txt","r");
        do
        {
            fseek(fichier, 7, SEEK_CUR);
            fgets(message, 20, fichier);
            message[strlen(message)-1]=0;

            fseek(fichier, 7, SEEK_CUR);
            fscanf(fichier, "%d\n", &points);

            if(strcmp(message, tab[i].pseudo)==0) //Même nom
            {
                if(confirmer("Reprendre votre score?"))
                    tab[i].score += points; //On reprend les points
            }
        }
        while(!feof(fichier));
        fclose(fichier);
    }
}

void prendreLogo(t_joueur joueur[8], t_image nain[8], int numJoueur, t_image sortie[2]) //Le joueur prend son icone
{
    int afficher=1, i, j, choix=0, sortir=0, icone=0;
    BITMAP *buffer;
    buffer = create_bitmap(800, 600);
    BITMAP *fond;
    fond = load_bitmap("Autres/fond.bmp", NULL);
    BITMAP *souris;
    souris = load_bitmap("Autres/souris.bmp", NULL);
    BITMAP *cadre;
    cadre = load_bitmap("Autres/cadre.bmp", NULL);

    do
    {

        blit(fond, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage des fonds, cadres et textes
        stretch_sprite(buffer, cadre, 0, 380, 800, 200);
        stretch_sprite(buffer, cadre, 550, 0, 250, 200);
        textprintf_ex(buffer, font, 560, 10, makecol(0, 0, 0), -1, "Joueur %d: %s", numJoueur+1, joueur[numJoueur].pseudo);
        textprintf_ex(buffer, font, 560, 30, makecol(0, 0, 0), -1, "Choisissez: icone nom:");

        choix=0;
        sortir=0;

        for(i=0; i<8; i++)  //Afficher les icones restantes
        {
            afficher=1;
            for(j=0; j<numJoueur; j++)   //Detecter si l'icone est choisie
            {
                if(joueur[j].logo==i)
                    afficher=0;
            }

            if(afficher==1) //Si l'icone est disponible (=pas prise)
            {
                stretch_sprite(buffer, nain[i].fichier, nain[i].pos_x, nain[i].pos_y, nain[i].taille_x, nain[i].taille_y);
                if(mouse_x>nain[i].pos_x && mouse_x<nain[i].pos_x+nain[i].taille_x && mouse_y>nain[i].pos_y && mouse_y<nain[i].pos_y+nain[i].taille_y)
                {
                    choix=i+1;
                    rect(buffer, nain[i].pos_x-5, nain[i].pos_y-5,nain[i].pos_x+5+nain[i].taille_x+5, nain[i].pos_y+nain[i].taille_y+5, makecol(215, 154, 16));
                }
            }
        }

        if((mouse_b & 1) && choix>0)//Confirmer le choix d'icone
            icone=choix;

        for(i=1; i<9; i++)//Afficher l'image cliquée
        {
            if(i==icone)
                stretch_sprite(buffer, nain[i-1].fichier, 620, 50, nain[i-1].taille_x, nain[i-1].taille_y);
        }

        //Pseudo
        if(mouse_x>550 && mouse_x<800 && mouse_y>0 && mouse_y<60 && (mouse_b & 1))
            ChoixPseudo(&joueur[numJoueur], cadre);

        //detecter les conditions de validation:
        if(joueur[numJoueur].pseudo[0]!='N' && icone>0)  //Le nom a été modifié et un image est choisie
            sortir=1;

        if(mouse_x>=755 && mouse_x<800 && mouse_y>155 && mouse_y<200 && (mouse_b & 1) && sortir==1) //Sortir
            sortir=5;

        if(sortir==1 || sortir==0)
            draw_sprite(buffer, sortie[sortir].fichier, sortie[sortir].pos_x, sortie[sortir].pos_y);
        draw_sprite_h_flip(buffer, souris, mouse_x-2, mouse_y-20);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
    while(sortir!=5);  //Sortir quand le joueur confirme

    destroy_bitmap(fond);
    destroy_bitmap(buffer);
    destroy_bitmap(souris);
    destroy_bitmap(cadre);

    joueur[numJoueur].logo=icone-1;
}


void initSortie(t_image sortie[2])
{
    int i;

    for(i=0; i<2; i++)
    {
        sprintf(sortie[i].nomfichier, "Autres/sortie%d.bmp", i);
        sortie[i].fichier = load_bitmap(sortie[i].nomfichier, NULL);
        sortie[i].pos_x=756;
        sortie[i].pos_y=156;
        sortie[i].taille_x=40;
        sortie[i].taille_y=40;

        if(!sortie[i].fichier)
            allegro_message("Pb %s", sortie[i].nomfichier);
    }
}


//Telechargement des images et initialisation des coordonnées pour l'affichage de l'avant nouvelle partie
void initialisation_image_nouvelle_partie(t_image* element)
{
    int i;
    int pos_x=250;
    int taille_x = 390;
    int pos_y = 430;
    int taille_y = 460;

    ///Fond d'écran
    sprintf(element[0].nomfichier,"element/fond.bmp");                 //Ecriture de l'adresse de l'image dans nomfichier
    element[0].fichier = load_bitmap(element[0].nomfichier, NULL);           //Téléchargement de l'image
    element[0].pos_x = 0;
    element[0].pos_y = 0;
    element[0].taille_x = 800;
    element[0].taille_y = 600;
    if(!element[0].fichier)                                       //Blindage sur le téléchargement du fond
        allegro_message("Erreur : L'image <fond> n'a pas ete correctement telecharger !");

    sprintf(element[1].nomfichier,"element/retour.bmp");
    sprintf(element[2].nomfichier,"element/retour2.bmp");
    sprintf(element[3].nomfichier,"element/suivant.bmp");
    sprintf(element[4].nomfichier,"element/suivant2.bmp");

    for(i=1; i<3; i++)
    {
        ///Element non selectionné
        element[i].fichier = load_bitmap(element[i].nomfichier, NULL);     //Téléchargement de l'image
        element[i].pos_x = pos_x;
        element[i].pos_y = pos_y;
        element[i].taille_x = taille_x;
        element[i].taille_y = taille_y;

        if(!element[i].fichier)                                  //Blindage sur le téléchargement de l'image
            allegro_message("Erreur : L'image <%s> n'a pas ete correctement telecharger !",element[i].nomfichier);
    }

    pos_x = taille_x+20;
    taille_x = pos_x+140;

    for(i=3; i<5; i++)
    {
        ///Element non selectionné
        element[i].fichier = load_bitmap(element[i].nomfichier, NULL);     //Téléchargement de l'image
        element[i].pos_x = pos_x;
        element[i].pos_y = pos_y;
        element[i].taille_x = taille_x;
        element[i].taille_y = taille_y;

        if(!element[i].fichier)                                  //Blindage sur le téléchargement de l'image
            allegro_message("Erreur : L'image <%s> n'a pas ete correctement telechargee !",element[i].nomfichier);
    }

    ///Case nombre de joueur non selectionnée
    sprintf(element[5].nomfichier,"element/nb joueur.bmp");                 //Ecriture de l'adresse de l'image dans nomfichier
    element[5].fichier = load_bitmap(element[5].nomfichier, NULL);           //Téléchargement de l'image
    element[5].pos_x = 250;
    element[5].pos_y = 300;
    element[5].taille_x = 550;
    element[5].taille_y = 330;
    if(!element[5].fichier)                                       //Blindage sur le téléchargement de l'image
        allegro_message("Erreur : L'image <nb joueur> n'a pas ete correctement telecharger !");

    ///Case nombre de joueur en rouge
    sprintf(element[22].nomfichier,"element/nb joueur2.bmp");                 //Ecriture de l'adresse de l'image dans nomfichier
    element[22].fichier = load_bitmap(element[22].nomfichier, NULL);           //Téléchargement de l'image
    element[22].pos_x = 250;
    element[22].pos_y = 300;
    element[22].taille_x = 550;
    element[22].taille_y = 330;
    if(!element[22].fichier)                                       //Blindage sur le téléchargement de l'image
        allegro_message("Erreur : L'image <nb joueur> n'a pas ete correctement telecharger !");

    ///On fait la même chose qu'avec les cases précédentes mais pour chaque nombre de joueur
    initialisation_choix_nb_joueur(element);

}


//Telechargement des images et initialisation des coordonnées pour l'affichage du choix du nombre de joueur
void initialisation_choix_nb_joueur(t_image* element)
{
    //Variables permettant de gérer la position de l'image
    int pos_x = 240;
    int taille_x = 277;
    int ecart = 10;
    int pos_y = 355;
    int taille_y = 395;

    //Variable permettant de gerer le télechargement d'images
    int i;
    int j = 1;
    int res = 0;

    for(i=6; i<22; i++)
    {
        /*Toutes les case paires du tableau element correspondent aux images sans effets de sélection
          et toutes les cases impaires correspondent aux images avec effet de sélection*/
        res = i%2;
        if(res == 0)
        {
            res = 2;
        }
        else
        {
            res = 3;
        }

        if(res == 2)
        {
            ///Numéro j non selectionnée
            //Ecriture de l'adresse de l'image dans nomfichier
            sprintf(element[i].nomfichier,"element/%da.bmp",j);
            //Téléchargement de l'image
            element[i].fichier = load_bitmap(element[i].nomfichier, NULL);
            element[i].pos_x = pos_x;
            element[i].pos_y = pos_y;
            element[i].taille_x = taille_x;
            element[i].taille_y = taille_y;
            if(!element[i].fichier)                                       //Blindage sur le téléchargement de l'image
                allegro_message("Erreur : L'image <%da> n'a pas ete correctement telechargee !",j);
        }

        if(res == 3)
        {
            ///Numéro j selectionnée
            //Ecriture de l'adresse de l'image dans nomfichier
            sprintf(element[i].nomfichier,"element/%db.bmp",j);
            //Téléchargement de l'image
            element[i].fichier = load_bitmap(element[i].nomfichier, NULL);
            element[i].pos_x = pos_x;
            element[i].pos_y = pos_y;
            element[i].taille_x = taille_x;
            element[i].taille_y = taille_y;
            if(!element[i].fichier)                                       //Blindage sur le téléchargement de l'image
                allegro_message("Erreur : L'image <%db> n'a pas ete correctement telechargee !",i);

            //Préparation des coordonnées pour l'image suivante
            pos_x = taille_x + ecart;
            taille_x = pos_x + 37;

            j++;
        }
    }
}

//Affichage d'une vidéo avant le lancement d'une nouvelle partie
void gif()
{
    ///Déclarations des variables utilisées
    BITMAP **frames = NULL;
    int *durations = NULL;
    BITMAP *fond = NULL;
    char nom_fichier[30];

    ///Téléchargement de l'image de chargement et blindage sur cette dernière
    sprintf(nom_fichier,"element/fond_chargement.bmp");
    fond = load_bitmap(nom_fichier,NULL);
    if(!fond)
    {
        allegro_message("Erreur fond");
    }

    ///Création d'un buffer
    BITMAP * buffer;
    buffer = create_bitmap(800, 600);

    int i = 0;
    int nb_image = 0;

    ///Affichage de l'image de chargement pendant pour ensuite laisser le temps au gif de se telecharger (environ 4 secondes)
    blit(fond,buffer,0,0,0,0,800,600);
    blit(buffer,screen,0,0,0,0,800,600);

    ///Téléchargement du gif
    sprintf(nom_fichier,"video/gif1.gif");
    nb_image = algif_load_animation (nom_fichier, &frames, &durations);

    ///Blindage sur le bon téléchargement du gif
    if (nb_image)
    {
        ///Affichage de l'image
        for (i = 0; i < nb_image; i++)
        {
            blit(frames[i],buffer,0,0,0,0,800,600);
            stretch_sprite(buffer,frames[i],-150,0,1100,600);
            blit(buffer,screen,0,0,0,0,800,600);
            rest(30);
        }

        ///Libération de l'espace mémoire utilisé
        for (i = 0; i < nb_image; i++)
            destroy_bitmap (frames[i]);
        free (frames);
        free (durations);
        destroy_bitmap(fond);
        destroy_bitmap(buffer);
    }
    else
        //Le téléchargement du gif a échoué
        allegro_message ("Erreur : le gif <video_gif> n'a pas ete correctement telecharge");
}


void meilleur_joueur()
{
///Déclaration des variables + ouverture du fichier et son blindage
    char pseudo[20], pseudo_meilleur[20];
    char nom_fichier[30];
    int score = 0, score_meilleur = 0;
    int logo = 0, logo_meilleur = 0, touche=0;

    FILE* fichier = NULL;
    BITMAP *image[2];
    BITMAP *buffer = NULL;

    buffer = create_bitmap(800,600);
    fichier = fopen("sauvegarde_score.txt","r");

    ///Telechargement de l'image de texte
    sprintf(nom_fichier,"Autres/meilleur score.bmp");
    image[0] = load_bitmap(nom_fichier,NULL);
    if(!image[0])
        allegro_message("erreur chargement image meileur score");

    //Telechargement du logo du gagnant
    sprintf(nom_fichier,"Nains/nain %d.bmp",logo+1);
    image[1] = load_bitmap(nom_fichier,NULL);
    if(!image[1])
        allegro_message("erreur chargement nain %d", logo+1);

    if(fichier == NULL || buffer==NULL)
    {
        allegro_message("Erreur de declaration dans affichage des scores !\n");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    else
    {
        while(!feof(fichier))
        {
//Permet d'effectuer des comparaisons
            fseek(fichier, 7, SEEK_CUR);
            fscanf(fichier,"%s\n",pseudo); //Lecture du pseudo
            fseek(fichier, 7, SEEK_CUR);
            fscanf(fichier,"%d\n",&score); //lecture du score
            fseek(fichier, 7, SEEK_CUR);
            fscanf(fichier,"%d\n",&logo); //Lecture du logo

///Si on trouve quelqu'un qui possède un plus gros score, on actualise les données du gagnant
            if(score > score_meilleur)
            {
                score_meilleur = score;
                logo_meilleur = logo;
                strcpy(pseudo_meilleur,pseudo);


                //Telechargement du logo du gagnant
                sprintf(nom_fichier,"Nains/nain %d.bmp",logo_meilleur);
                image[1] = load_bitmap(nom_fichier,NULL);
                if(!image[1])
                    allegro_message("erreur chargement nain %d", logo_meilleur);

                //allegro_message("%s : %d, %d", pseudo_meilleur, score_meilleur, logo_meilleur);
            }
        }

        do
        {
            if(key[KEY_ESC])
                touche=1;

            //Affichage du texte "Le meilleur joueur est"
            draw_sprite(buffer,image[0],200,50);
            //Affichage du logo du gagnant
            stretch_sprite(buffer,image[1],350,150,221,468);

            //Affichage du pseudo et du score du gagnant
            textprintf_ex(buffer, font, 200, 90, makecol(255, 255, 255), -1, "%s avec %d points ! ",pseudo_meilleur,score_meilleur);

            //Affichage du buffer
            blit(buffer, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
        }
        while(key[KEY_ESC] || touche==0); //Sortir = lacher esc
    }

    fclose(fichier);
    destroy_bitmap(buffer);
    destroy_bitmap(image[0]);
    destroy_bitmap(image[1]);
}
