#include "header.h"

void MainPlateau(int nbJoueurs, BITMAP *nain, t_joueur joueur[8], t_Pile *pile, BITMAP *plateau, int pepite[3], char defausse[10], int ordi, int tour)   //Le 1er programme pour les choix du personnage
{
    int i, sortie=0;

    do
    {
        for(i=0; i<nbJoueurs; i++) ///Un tour pour chaque joueur
        {
            if(tour!=0)//Reprendre le tour du joueur d'une partie interrompue
            {
                i=tour;
                tour=0;
            }
            if(i==1 && ordi==1)//Tour d'ordi
                TourOrdi(joueur, pile, nbJoueurs, plateau, defausse);
            else
                Plateau(joueur, nain, pile, nbJoueurs, i, plateau, pepite, defausse, &sortie);

            if(sortie==1)
            {
                tour=i;
                i=nbJoueurs;
            }
        }
    }
    while(sortie==0);

    if(sortie==1) //Sauvegarder
    {
        Sauvegarder(joueur, nbJoueurs, tour, pepite, ordi, plateau);
        SauvegarderPile(pile);
        sauvegarde_score(joueur, nbJoueurs);
    }
}


void Plateau(t_joueur joueur[8], t_image nain[8], t_Pile *pile, int nbJoueurs, int numJ, BITMAP *plateau, int pepite[3], char defausse[10], int *sortie)    //initialise les variables des joueurs
{
    int dragDrop=0, actions=0, cartePrise=-1, zoom=0, touche=0, zoomX, zoomY, i;
    BITMAP *buffer;
    buffer = create_bitmap(800, 600);

    ///IMAGES:
    BITMAP *fond;
    BITMAP *souris;
    BITMAP *passer[2];
    BITMAP *ImPepite[3];
    BITMAP *outils[3];
    t_image *dos;
    t_image *galerie;
    t_image *action;
    dos = (t_image*)malloc(sizeof(t_image)*3);
    galerie = (t_image*)malloc(sizeof(t_image)*6);
    action = (t_image*)malloc(sizeof(t_image)*12);
    initCartes(dos, galerie, action);   //Initialise les cartes
    fond = load_bitmap("Autres/terrain.bmp", NULL);
    souris = load_bitmap("Autres/souris.bmp", NULL);
    ImPepite[0] = load_bitmap("cartes/1pepite.bmp", NULL);
    ImPepite[1] = load_bitmap("cartes/2pepite.bmp", NULL);
    ImPepite[2] = load_bitmap("cartes/3pepite.bmp", NULL);
    outils[0] = load_bitmap("Autres/lanterne.bmp", NULL);
    outils[1] = load_bitmap("Autres/charette.bmp", NULL);
    outils[2] = load_bitmap("Autres/pioche.bmp", NULL);
    passer[0] = load_bitmap("Autres/passer0.bmp", NULL);
    passer[1] = load_bitmap("Autres/passer1.bmp", NULL);

    do  ///-------------BOUCLE DU JEU-------------/
    {
        blit(fond, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage des fonds, cadres et textes
        textprintf_ex(buffer, font, 5, 5, makecol(0, 0, 0), -1, "Joueur: %s", joueur[numJ].pseudo);
        textprintf_ex(buffer, font, 165, 5, makecol(0, 0, 0), -1, "Score: %d", joueur[numJ].score);
        stretch_sprite(buffer, nain[joueur[numJ].logo].fichier, 400, 5, 48, 62);//Affichage logo

        afficherCartes(pile, dos, galerie, action, outils, passer, joueur[numJ], cartePrise, defausse, buffer);
        afficherPlateau(galerie, action, buffer, plateau);
        afficherAdversaires(joueur, nbJoueurs, numJ, nain, buffer);

        ///Les actions du joueur : dans l'ordre
        ActionsJoueur(joueur, pile, &dragDrop, &actions, &cartePrise, defausse, numJ, nbJoueurs, dos, action, galerie, buffer, plateau, ImPepite, pepite);

        if(dragDrop==0)
            VoirJoueurs(joueur, nain, outils, numJ, nbJoueurs);

        draw_sprite_h_flip(buffer, souris, mouse_x-2, mouse_y-20);  //Afficher souris

        if(key[KEY_P])
            *sortie=1;
        ///ZOOM
        if(key[KEY_SPACE])//Permet changer le zoom quand espace n'est plus appuyé
            touche=1;

        if(!key[KEY_SPACE] && touche==1)//Changement Zoom/pas Zoom
        {
            touche=0;
            if(zoom==1)
                zoom=0;
            else
                zoom=1;
        }

        if(zoom==0)
            blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        if(zoom==1)
        {

            zoomX = mouse_x-300;
            zoomY = mouse_y-200;
            if(zoomX<0)
                zoomX=0;
            if(zoomY<0)
                zoomY=0;
            if(zoomX>160)
                zoomX=160;
            if(zoomY>120)
                zoomY=120;
            stretch_blit(buffer, screen, zoomX, zoomY, 800, 600, 0, 0, 800+200, 600+150);
        }
    }
    while(actions!=3 && *sortie==0);  //Sortir quand le joueur confirme ou sauvegarde

    destroy_bitmap(fond);
    destroy_bitmap(buffer);
    destroy_bitmap(souris);
    destroy_bitmap(passer[0]);
    destroy_bitmap(passer[1]);
    for(i=0; i<3; i++)
    {
        destroy_bitmap(outils[i]);
        destroy_bitmap(ImPepite[i]);
    }

    ///Détecter la fin de partie
    if(*sortie!=1)//Sortie par fin de tour
        FinDePartie(joueur, numJ, nbJoueurs, sortie, plateau, pepite, pile);
}



void VoirJoueurs(t_joueur joueur[8], t_image nain[8], BITMAP *outils[3], int joueurActuel, int nbJoueur)
{
    int i, ecart, ordre;
    ecart = 420/(nbJoueur-1);

    if(mouse_x>=0 && mouse_x<=90 && mouse_b&1)
    {
        for(i=0; i<nbJoueur; i++)
        {
            if(mouse_y>=160+i*ecart && mouse_y<160+ecart+i*ecart)
            {
                ordre = joueurActuel+i+1;
                if(ordre>=nbJoueur)
                    ordre = ordre-nbJoueur;
                AfficherJoueur(joueur[ordre], nain, outils);
            }
        }
    }
}

void AfficherJoueur(t_joueur joueur, t_image nain[8], BITMAP *outils[3])
{
    int i;
    BITMAP *buffer;
    buffer = create_bitmap(200, 200);

    BITMAP *fond;
    fond = load_bitmap("confirmer/fond.bmp", NULL);

    do
    {
        //Afficher: fond, logo, et pénalités
        stretch_sprite(buffer, fond, 0,0, 200,200);
        stretch_sprite(buffer, nain[joueur.logo].fichier, 60,30, 70,100);
        for(i=0; i<3; i++)
        {
            if(joueur.outils[i]==0)//Outil cassé
                stretch_sprite(buffer, outils[i], 30+50*i,130, 40,40);
        }


        blit(buffer, screen, -100,-300,0,0, SCREEN_W, SCREEN_H);
    }
    while(!key[KEY_ESC]);
    destroy_bitmap(buffer);
    destroy_bitmap(fond);
}

void afficherAdversaires(t_joueur joueur[8], int nbJoueur, int joueurActuel, t_image nains[8], BITMAP *buffer)
{
    int i, ecart, ordre;
    float reduction;
    ecart = 420/(nbJoueur-1);

    if(nbJoueur>=5)
        reduction=0.7;
    else
        reduction=1;

    for(i=0; i<nbJoueur; i++)
    {
        ordre = joueurActuel+i+1;   //Ordre permet d'afficher les joueurs dans l'ordre des joueurs suivant le joueur actif
        if(ordre>=nbJoueur)
            ordre = ordre-nbJoueur;
        line(buffer, 0, 163+i*ecart, 90, 163+i*ecart, makecol(80, 255, 80));//Ligne verte séparant les espaces des joueurs

        if(ordre!=joueurActuel)//Ne pas afficher le joueur qui joue
            stretch_sprite(buffer, nains[joueur[ordre].logo].fichier, 10, 160+10+i*ecart, 60*reduction,70*reduction);
    }
}



void afficherCartes(t_Pile *pile, t_image dos[3], t_image galerie[6], t_image action[12], BITMAP *outils[3], BITMAP *passer[2], t_joueur joueur, int cartePrise, char defausse[10], BITMAP *buffer)
{
    int i, j;
    char carte[10];

    ///Cartes pépites et pioche, et passer
    if(pile->premier!=NULL)
        stretch_sprite(buffer, dos[2].fichier, dos[2].pos_x, dos[2].pos_y, dos[2].taille_x, dos[2].taille_y);
    stretch_sprite(buffer, dos[1].fichier, 710, 190, dos[2].taille_x, dos[2].taille_y);
    stretch_sprite(buffer, dos[1].fichier, 710, 190+137, dos[2].taille_x, dos[2].taille_y);
    stretch_sprite(buffer, dos[1].fichier, 710, 190+2*137, dos[2].taille_x, dos[2].taille_y);
    stretch_sprite(buffer, passer[0], 620, 0, 130, 30);
    if(mouse_x>620 && mouse_x<750 && mouse_y<30)
        stretch_sprite(buffer, passer[1], 620, 0, 130, 30);


    ///La carte de défausse
    if(defausse[0] == 'A')//Carte action:
    {
        if(defausse[3]=='0')//Action 10
            stretch_sprite(buffer, action[10].fichier, dos[2].pos_x+110, dos[2].pos_y, dos[2].taille_x, dos[2].taille_y);
        else if(defausse[3]=='1')//Action 11
            stretch_sprite(buffer, action[11].fichier, dos[2].pos_x+110, dos[2].pos_y, dos[2].taille_x, dos[2].taille_y);
        else
            stretch_sprite(buffer, action[defausse[2]-48].fichier, dos[2].pos_x+110, dos[2].pos_y, dos[2].taille_x, dos[2].taille_y);
    }
    if(defausse[0] == 'G')//Carte galerie:
    {
        stretch_sprite(buffer, galerie[defausse[2]-48].fichier, dos[2].pos_x+110, dos[2].pos_y, dos[2].taille_x, dos[2].taille_y);
    }

    ///Malus du joueur:
    for(i=0; i<3; i++)
    {
        if(joueur.outils[i]==0)//L'outils est cassé
            stretch_sprite(buffer, outils[i], 10+80*i, 15+action[0].taille_y+5, 30, 30);
    }
    ///Jeu du joueur
    for(i=0; i<7; i++)
    {
        if(i!=cartePrise)//On n'affiche dans la main du joueur les cartes qui ne sont pas prises
        {
            for(j=0; j<12; j++)
            {
                sprintf(carte, "A %d", j);
                if(strcmp(joueur.cartes[i],carte)==0)
                    stretch_sprite(buffer, action[j].fichier, 10+(action[j].taille_x+5)*i, 15, action[j].taille_x, action[j].taille_y);
            }
            for(j=0; j<6; j++)
            {
                sprintf(carte, "G %d", j);
                if(strcmp(joueur.cartes[i],carte)==0)
                    stretch_sprite(buffer, galerie[j].fichier, 10+(action[j].taille_x+5)*i, 15, action[j].taille_x, action[j].taille_y);    //On utilise la taille des cartes actions
            }
        }
    }
}


void afficherPlateau(t_image galerie[6], t_image action[12], BITMAP *buffer, BITMAP *plateau)
{
    int numCarte, col, i, j;

    //Afficher la carte de départ
    stretch_sprite(buffer, galerie[0].fichier, 180-74, 165+2*84, 74, 84);

    ///Aligner la carte sur le plateau
    for(i=0; i<7; i++)
    {
        for(j=0; j<5; j++)
        {
            ///Déterminer la carte placée sur le plateau en comptant les pixels
            numCarte=-1;
            do
            {
                numCarte++;
                col = getpixel(plateau, 180+74/2+i*74+numCarte, 165+84/2+j*84);
                col = getg(col);
            }
            while(col==255);

            ///Afficher la carte correspondante
            if(numCarte>0 && numCarte<6)//Galeries
                stretch_sprite(buffer, galerie[numCarte].fichier, 180+i*74, 165+j*84, 74, 84);
            if(numCarte==6)//L'éboulement
                stretch_sprite(buffer, action[10].fichier, 180+i*74, 165+j*84, 74, 84);
        }
    }
}


void ActionsJoueur(t_joueur joueur[8], t_Pile *pile, int *dragDrop, int *actions, int *cartePrise, char defausse[10], int numJ, int nbJoueur, t_image dos[3], t_image action[12], t_image galerie[6], BITMAP *buffer, BITMAP *plateau, BITMAP *ImPepite[3], int pepite[3])
{
    int i, nbCartes=0;

    ///Compter les cartes du joueur
    for(i=0; i<7; i++)
    {
        if(strcmp(joueur[numJ].cartes[i], "0")!=0)//La carte existe
            nbCartes++;
    }
    if(*actions<=0)///--------------------Le joueur doit piocher/défausser--------------------///
    {
        if(nbCartes==7 || *actions<0)
            rect(buffer, 710, 30, 784, 114, makecol(255, 20, 20));
        else if (pile->premier!=NULL)
            rect(buffer, 600, 30, 674, 114, makecol(60, 255, 60));

        //Le joueur clique sur la carte de la pile
        if((*dragDrop==0 || *dragDrop==2) && mouse_x>=dos[2].pos_x && mouse_x<=dos[2].pos_x+dos[2].taille_x && mouse_y>=dos[2].pos_y && mouse_y<=dos[2].pos_y+dos[2].taille_y && (mouse_b & 1))
            *dragDrop=1;
        //Le joueur prend une de ses cartes
        if(mouse_y>=15 && mouse_y<=15+action[0].taille_y)
        {
            for(i=0; i<7; i++)
            {
                if(mouse_x>=10+(action[0].taille_x+5)*i && mouse_x<=10+(action[0].taille_x+5)*(i+1) && *cartePrise==-1 && mouse_b&1)//La souris se situe sur une des 7 cartes. Aucune carte n'est déjà prise par le joueur
                {
                    *cartePrise = i;
                    *dragDrop=1;
                }
            }
        }

        ///Drag and drop de la pioche
        if(*actions==0 && *cartePrise==-1 && nbCartes<7 && pile->premier!=NULL)
        {
            dragAndDrop(dos[2], 0, 0, 590, 130, dragDrop, buffer);
            if(*dragDrop==2) //Succès
            {
                *actions=1; //le joueur a pioché
                for(i=0; i<7; i++)
                {
                    if(strcmp(joueur[numJ].cartes[i], "0")==0)
                        strcpy(joueur[numJ].cartes[i], piocher(pile));
                }
                *dragDrop=0;
            }
        }
        ///Drag and drop: défausser un carte: action ou galerie
        if(joueur[numJ].cartes[*cartePrise][0]=='A' && pile->premier!=NULL) //Carte action
        {

            ///On effectuer un drag and drop des cartes A 0 à A 9, et des cartes A 10, A 11:
            if(strcmp(joueur[numJ].cartes[*cartePrise], "A 11")==0)///Carte map
                dragAndDrop(action[11], 700, 30, 90, 80, dragDrop, buffer);
            else if(strcmp(joueur[numJ].cartes[*cartePrise], "A 10")==0)///Carte éboulement
                dragAndDrop(action[10], 700, 30, 90, 80, dragDrop, buffer);
            else
                dragAndDrop(action[joueur[numJ].cartes[*cartePrise][2]-48], 700, 30, 90, 80, dragDrop, buffer);

            if(*dragDrop==2) //Carte défausée
            {
                (*actions)--;//Le joueur veut défausser 3 cartes
                strcpy(defausse, joueur[numJ].cartes[*cartePrise]); //Défauser la carte
                sprintf(joueur[numJ].cartes[*cartePrise], "0");
            }
        }
        if(joueur[numJ].cartes[*cartePrise][0]=='G' && pile->premier!=NULL) //Carte galerie
        {
            dragAndDrop(galerie[joueur[numJ].cartes[*cartePrise][2]-48], 700, 30, 90, 80, dragDrop, buffer);

            if(*dragDrop==2) //Carte défausée
            {
                (*actions)--;//Le joueur veut défausser 3 cartes
                strcpy(defausse, joueur[numJ].cartes[*cartePrise]); //Défauser le carte
                sprintf(joueur[numJ].cartes[*cartePrise], "0");
            }
        }
        if(*actions==-3)//Trois cartes ont été défaussée
        {
            for(i=0; i<6; i++)//Piocher trois cartes
            {
                if(strcmp(joueur[numJ].cartes[i],"0")==0 && pile->premier!=NULL)
                    strcpy(joueur[numJ].cartes[i], piocher(pile));
            }
            *actions=2;//Tour suivant
        }
        if(!(mouse_b&1))    //Lâcher la carte
            *cartePrise=-1;
    }

    if(*actions==1)///--------------------Le joueur doit placer une carte ou passer--------------------///
    {
        if(mouse_y>=15 && mouse_y<=15+action[0].taille_y)
        {
            for(i=0; i<7; i++)
            {
                if(mouse_x>=10+(action[0].taille_x+5)*i && mouse_x<=10+(action[0].taille_x+5)*(i+1) && *cartePrise==-1)//La souris se situe sur une des 7 cartes. Aucune carte n'est déjà prise par le joueur
                {
                    *cartePrise = i;
                }
            }
        }
        if(joueur[numJ].cartes[*cartePrise][0]=='G')///On utilise une carte galerie,
        {
            *dragDrop=1;
            dragAndDrop(galerie[joueur[numJ].cartes[*cartePrise][2]-48], 180, 165, 520, 420, dragDrop, buffer);
            if(*dragDrop!=0)
                verifierPlateau((joueur[numJ].outils[0]+joueur[numJ].outils[1]+joueur[numJ].outils[2]), joueur[numJ].cartes[*cartePrise][2]-48, dragDrop, buffer, plateau);

            if(*dragDrop==2) //Succès du drag and drop
            {
                *actions=2; //le joueur peut ensuite finir son tour
                PlacerCarte(joueur[numJ].cartes[*cartePrise][2]-48, plateau);
                sprintf(joueur[numJ].cartes[*cartePrise], "0");
            }
        }
        if(joueur[numJ].cartes[*cartePrise][0]=='A')///On utilise une carte action:
        {
            *dragDrop=1;

            ///Type de la carte:
            if(joueur[numJ].cartes[*cartePrise][3]-48!=0 && joueur[numJ].cartes[*cartePrise][3]-48!=1) ///Carte détruire, ou réparer = de A0 à A9 (!=A10 et A11)
            {
                //dragAndDrop(action[joueur[numJ].cartes[*cartePrise][2]-48], 180, 165, 520, 420, dragDrop, buffer);//Donner à l'adversaire

                if(joueur[numJ].cartes[*cartePrise][2]-48<=5 && mouse_y<150) ///Carte réparer (sur soi)
                {
                    dragAndDrop(action[joueur[numJ].cartes[*cartePrise][2]-48], 0, 70, 300, 70, dragDrop, buffer);

                    if(*dragDrop==2)//Succès
                    {
                        Reparer(joueur[numJ].cartes[*cartePrise][2]-48, &joueur[numJ], dragDrop);
                        if(*dragDrop==2)//Carte utilisée
                        {
                            strcpy(defausse, joueur[numJ].cartes[*cartePrise]); //Défauser le carte
                            sprintf(joueur[numJ].cartes[*cartePrise], "0");
                            *actions=2;
                            *dragDrop=0;
                        }
                        else
                        {
                            afficherMessage("Cette carte n'aura pas d'effet");
                        }
                    }
                }
                else ///Carte Détruire et réparer (sur les adversaires)
                {
                    dragAndDrop(action[joueur[numJ].cartes[*cartePrise][2]-48], 0, 160, 90, 420, dragDrop, buffer);
                    if(*dragDrop==2)//Succès
                    {
                        outilsAdverses(joueur, nbJoueur, numJ, joueur[numJ].cartes[*cartePrise][2]-48, dragDrop);   ///Va permettre de modifier l'état des outils du joueur ciblé
                        if(*dragDrop==2)//Carte utilisée
                        {
                            strcpy(defausse, joueur[numJ].cartes[*cartePrise]); //Défauser la carte
                            sprintf(joueur[numJ].cartes[*cartePrise], "0");
                            *actions=2;
                            *dragDrop=0;
                        }
                        else
                        {
                            afficherMessage("Cette carte n'aura pas d'effet");
                        }
                    }
                }
            }
            if(strcmp(joueur[numJ].cartes[*cartePrise], "A 10")==0) ///Carte éboulement: Placer sur le plateau
            {
                dragAndDrop(action[10], 180, 165, 520, 420, dragDrop, buffer);

                if(*dragDrop!=0)
                    eboulement(dragDrop, buffer, plateau);  //On vérifie que la carte éboulement se situe sur une autre carte

                if(*dragDrop==2)//Succès
                {
                    *actions=2; //le joueur peut ensuite finir son tour
                    detruireGalerie(plateau);
                    strcpy(defausse, joueur[numJ].cartes[*cartePrise]); //Défauser le carte
                    sprintf(joueur[numJ].cartes[*cartePrise], "0");
                }
            }
            if(strcmp(joueur[numJ].cartes[*cartePrise], "A 11")==0) ///Carte map
            {
                dragAndDrop(action[11], 700, 180, 100, 380, dragDrop, buffer);

                if(*dragDrop==2)//Succès
                {
                    i = mouse_y-180;//Trouver la carte à retourner
                    if(i<126)
                        i=0;
                    else if(i>254)
                        i=2;
                    else
                        i=1;
                    do
                    {
                        stretch_sprite(buffer, ImPepite[pepite[i]-1], 710, 190+i*137, 74, 84);
                        blit(buffer, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
                    }
                    while(!key[KEY_ESC]);

                    *actions=2;
                    strcpy(defausse, joueur[numJ].cartes[*cartePrise]); //Défauser le carte
                    sprintf(joueur[numJ].cartes[*cartePrise], "0");
                }
            }

        }

        if(!(mouse_b & 1)) //Le joueur ne clique pas sur la carte
        {
            *cartePrise=-1; //Aucune carte n'est prise
            *dragDrop=0;
        }

        if(mouse_b & 1 && mouse_x>620 && mouse_x<750 && mouse_y<30) ///Passer le tour sans jouer
        {
            if(("Passer le tour?"))//Demander la confirmation
                *actions=3;
        }
    }

    if(*actions==2)///Temps du joueur pour finir le tour
    {
        if(mouse_b & 1 && mouse_x>620 && mouse_x<750 && mouse_y<30)///Fin de tour
            *actions=3;
    }
}



int confirmer(char phrase[30])
{
    int choix, i;
    BITMAP *buffer;
    buffer = create_bitmap(300, 200);
    BITMAP *fond;
    BITMAP *souris;
    BITMAP *bouton[4];
    fond = load_bitmap("confirmer/fond.bmp", NULL);
    souris = load_bitmap("Autres/souris.bmp", NULL);
    bouton[0] = load_bitmap("confirmer/oui0.bmp", NULL);
    bouton[1] = load_bitmap("confirmer/oui1.bmp", NULL);
    bouton[2] = load_bitmap("confirmer/non0.bmp", NULL);
    bouton[3] = load_bitmap("confirmer/non1.bmp", NULL);

    do
    {
        choix=-1;
        if(mouse_y-200>=120 && mouse_y-200<=140)
        {
            if(mouse_x-300>=50 && mouse_x-300<=110)//Oui
                choix=1;
            if(mouse_x-300>=200 && mouse_x-300<=260)//Non
                choix=0;
        }
        draw_sprite(buffer, fond, 0, 0);//Fond et contours
        rect(buffer, 0,0, 299, 199, makecol(0,0,0));
        if(choix==1)
            stretch_sprite(buffer, bouton[1], 50, 120, 60, 20);
        else
            stretch_sprite(buffer, bouton[0], 50, 120, 60, 20);
        if(choix==0)
            stretch_sprite(buffer, bouton[3], 200, 120, 60, 20);
        else
            stretch_sprite(buffer, bouton[2], 200, 120, 60, 20);

        textprintf_ex(buffer, font, 40, 50, makecol(255, 255, 255), -1, "%s", phrase);
        draw_sprite_h_flip(buffer, souris, mouse_x-302, mouse_y-220);  //Afficher souris

        blit(buffer, screen, -250,-200,0,0, SCREEN_W, SCREEN_H);
    }
    while(choix==-1 || !(mouse_b&1));  //On attend que le joueur aie sa souris sur OUI ou NON et clique
    destroy_bitmap(buffer);
    destroy_bitmap(fond);
    destroy_bitmap(souris);
    for(i=0; i<4; i++)
        destroy_bitmap(bouton[i]);

    return choix;
}



void outilsAdverses(t_joueur joueur[8], int nbjoueur, int joueurActuel, int carte, int *dragDrop)
{
    int cible=0, ecartJoueurs, i;    //L'écart d'affichage entre les joueurs
    ecartJoueurs = 420/(nbjoueur-1);

    ///Déterminer la cible
    for(i=0; i<nbjoueur; i++)
    {
        if(mouse_y>=160+i*ecartJoueurs && mouse_y<=160+i*ecartJoueurs+ecartJoueurs)
            cible=joueurActuel+i+1;
    }
    if(cible>=nbjoueur)//Retrouver le bon joueur
        cible=cible-nbjoueur;

    if(carte<=5)///Carte Réparer
        Reparer(carte, &joueur[cible], dragDrop);
    if(carte>=6)///Carte détruire
        Detruire(carte, &joueur[cible], dragDrop);
}



void Detruire(int carte, t_joueur *joueur, int *dragDrop)
{
    int possible=0;

    if(carte==6 && joueur->outils[0]==1)///lanterne
    {
        joueur->outils[0]=0;
        possible=1;
    }
    if(carte==7 && joueur->outils[1]==1)///Charette
    {
        joueur->outils[1]=0;
        possible=1;
    }
    if(carte==8 && joueur->outils[2]==1)///Pioche
    {
        joueur->outils[2]=0;
        possible=1;
    }
    if(carte==9 && (joueur->outils[1]==1 || joueur->outils[2]==1 || joueur->outils[1]==1))///Tout casser
    {
        joueur->outils[0]=0;
        joueur->outils[1]=0;
        joueur->outils[2]=0;
        possible=1;
    }

    if(!possible)///Annuler l'utilisation de la carte si elle n'a aucun effet
        *dragDrop=0;
}



void Reparer(int carte, t_joueur *joueur, int *dragDrop)
{
    int possible=0;
    if(carte==0 && joueur->outils[0]==0)///lanterne
    {
        joueur->outils[0]=1;
        possible=1;
    }
    if(carte==1 && joueur->outils[1]==0)///Charette
    {
        joueur->outils[1]=1;
        possible=1;
    }
    if(carte==2 && joueur->outils[2]==0)///Pioche
    {
        joueur->outils[2]=1;
        possible=1;
    }
    if(carte==3 && (joueur->outils[1]==0 || joueur->outils[2]==0))///Pioche charette
    {
        joueur->outils[1]=1;
        joueur->outils[2]=1;
        possible=1;
    }
    if(carte==4 && (joueur->outils[1]==0 || joueur->outils[0]==0))///Lanterne charette
    {
        joueur->outils[1]=1;
        joueur->outils[0]=1;
        possible=1;
    }
    if(carte==5 && (joueur->outils[0]==0 || joueur->outils[2]==0))///Pioche lanterne
    {
        joueur->outils[0]=1;
        joueur->outils[2]=1;
        possible=1;
    }

    if(!possible)///Annuler l'utilisation de la carte si elle n'a aucun effet
        *dragDrop=0;
}



void dragAndDrop(t_image carte, int destX, int destY, int plusX, int plusY, int *dragDrop, BITMAP *buffer)
{
    if(!(mouse_b & 1) && *dragDrop==1)//Le joueur relache la carte
    {
        if(mouse_x>=destX && mouse_x<=destX+plusX && mouse_y>=destY && mouse_y<=destY+plusY) //La carte est amenée à sa destination
            *dragDrop=2;
        else    //La carte est relachée autre part
            *dragDrop=0;
    }
    if(*dragDrop==1)    //la carte est prise
        stretch_sprite(buffer, carte.fichier, mouse_x-(carte.taille_x)/2, mouse_y-(carte.taille_y)/2, carte.taille_x, carte.taille_y);
}

void initCartes(t_image dos[3], t_image galerie[6], t_image action[12])
{
    int i;

    for(i=0; i<3; i++)
    {
        sprintf(dos[i].nomfichier, "cartes/dos %d.bmp", i);
        dos[i].fichier = load_bitmap(dos[i].nomfichier,NULL);

        dos[i].taille_x = 74;
        dos[i].taille_y = 84;
        dos[i].pos_x = 600;
        dos[i].pos_y = 30;

        if(!dos[i].fichier)
            allegro_message("erreur: %s", dos[i].nomfichier);
    }
    for(i=0; i<6; i++)
    {
        sprintf(galerie[i].nomfichier, "cartes/galerie %d.bmp", i);
        galerie[i].fichier = load_bitmap(galerie[i].nomfichier,NULL);

        galerie[i].taille_x = 50;
        galerie[i].taille_y = 65;
        galerie[i].pos_x = 0;
        galerie[i].pos_y = 0;

        if(!galerie[i].fichier)
            allegro_message("erreur: %s", galerie[i].nomfichier);
    }
    for(i=0; i<12; i++)
    {
        sprintf(action[i].nomfichier, "cartes/action %d.bmp", i);
        action[i].fichier = load_bitmap(action[i].nomfichier,NULL);

        action[i].taille_x = 50;
        action[i].taille_y = 65;
        action[i].pos_x = 0;
        action[i].pos_y = 0;

        if(!action[i].fichier)
            allegro_message("erreur: %s", action[i].nomfichier);
    }
}



void PlacerCarte(int carte, BITMAP *plateau)
{
    int ligne=-100, col=-100, i, j;

    ///Aligner la carte sur le plateau
    for(i=0; i<7; i++)
    {
        for(j=0; j<5; j++)
        {
            if(mouse_x>=180+i*74 && mouse_x<=180+(i+1)*74 && mouse_y>=165+j*84 && mouse_y<=165+(j+1)*84)//Divise le plateau en 35 rectangles, trouve sur le quel se situe la souris
            {
                ligne=180 + i*74;
                col=165 + j*84;
            }
        }
    }
    ///Placer les pixels correspondant aux entrées
    if(carte==1)///Entrées : droite gauche
    {
        putpixel(plateau, ligne+73, col+84/2, makecol(255, 0, 0));
        putpixel(plateau, ligne, col+84/2, makecol(255, 0, 0));
    }
    if(carte==2)///Entrées : droite gauche haut
    {
        putpixel(plateau, ligne+73, col+84/2, makecol(255, 0, 0));
        putpixel(plateau, ligne, col+84/2, makecol(255, 0, 0));
        putpixel(plateau, ligne+74/2, col, makecol(255, 0, 0));
    }
    if(carte==3)///Entrées : droite bas haut
    {
        putpixel(plateau, ligne+73, col+84/2, makecol(255, 0, 0));
        putpixel(plateau, ligne+74/2, col+83, makecol(255, 0, 0));
        putpixel(plateau, ligne+74/2, col, makecol(255, 0, 0));
    }
    if(carte==4)///Entrées : droite gauche bas haut
    {
        putpixel(plateau, ligne+73, col+84/2, makecol(255, 0, 0));
        putpixel(plateau, ligne, col+84/2, makecol(255, 0, 0));
        putpixel(plateau, ligne+74/2, col+83, makecol(255, 0, 0));
        putpixel(plateau, ligne+74/2, col, makecol(255, 0, 0));
    }
    if(carte==5)///IMPASSE  =   entrée gauche
    {
        putpixel(plateau, ligne, col+84/2, makecol(255, 0, 0));
    }
    ///Placer les pixels indiquant le type de la carte
    for(i=0; i<carte; i++)
        putpixel(plateau, ligne+74/2+i, col+84/2, makecol(0, 255, 0)); //Indique le type de carte
}


void verifierPlateau(int nbOutils, int carte, int *dragDrop, BITMAP *buffer, BITMAP *plateau)
{
    int ligne=-100, col=-100, i, j, couleur, placer=-1;

    ///Aligner la carte sur le plateau
    for(i=0; i<7; i++)
    {
        for(j=0; j<5; j++)
        {
            if(mouse_x>=180+i*74 && mouse_x<=180+(i+1)*74 && mouse_y>=165+j*84 && mouse_y<=165+(j+1)*84)//Divise le plateau en 35 rectangles, trouve sur le quel se situe la souris
            {
                ligne=180 + i*74;
                col=165 + j*84;
                placer=1;
            }
        }
    }

    if(placer==1)///La carte est située dans le plateau
    {
        connectionCarte(plateau, ligne, col, &placer, carte);///Vérifie que la carte est bien adjacente à au moins une autre

        if(carte>=1 && carte<=4)///La carte possède un chemin à droite
        {
            couleur = getpixel(plateau, ligne+74/2+74, col+84/2);
            couleur = getg(couleur);

            if(couleur == 255)//L'emplacement de carte à droite est déjà utilisée
            {
                couleur = getpixel(plateau, ligne+74, col+84/2);
                couleur = getr(couleur);
                if(couleur!=255)//Le chemin n'est pas connecté
                    placer=0;
            }
        }
        else///Pas de chemin à droite
        {
            couleur = getpixel(plateau, ligne+74, col+84/2);
            couleur = getr(couleur);
            if(couleur==255)//Un chemin existe sur la carte de droite : la carte ne peut pas être placée
                placer=0;
        }
        if(carte==1 || carte==2 || carte==4 || carte==5)///La carte possède un chemin à gauche
        {
            couleur = getpixel(plateau, ligne+74/2-74, col+84/2);
            couleur = getg(couleur);

            if(couleur == 255)//L'emplacement de carte à gauche est déjà utilisée
            {
                couleur = getpixel(plateau, ligne-1, col+84/2);
                couleur = getr(couleur);
                if(couleur!=255)//Le chemin n'est pas connecté
                    placer=0;
            }
        }
        else///Pas de chemin à gauche
        {
            couleur = getpixel(plateau, ligne-1, col+84/2);
            couleur = getr(couleur);
            if(couleur==255)//Un chemin existe sur la carte de gauche : la carte ne peut pas être placée
                placer=0;
        }
        if(carte==2 || carte==3 || carte==4)///La carte possède un chemin en haut
        {
            couleur = getpixel(plateau, ligne+74/2, col+84/2-84);
            couleur = getg(couleur);

            if(couleur == 255)//L'emplacement de carte en haut est déjà utilisée
            {
                couleur = getpixel(plateau, ligne+74/2, col-1);
                couleur = getr(couleur);
                if(couleur!=255)//Le chemin n'est pas connecté
                    placer=0;
            }
        }
        else///Pas de chemin en haut
        {
            couleur = getpixel(plateau, ligne+74/2, col-1);
            couleur = getr(couleur);
            if(couleur==255)//Un chemin existe sur la carte du haut : la carte ne peut pas être placée
                placer=0;
        }
        if(carte==3 || carte==4)///La carte possède un chemin en bas
        {
            couleur = getpixel(plateau, ligne+74/2, col+84/2+84);
            couleur = getg(couleur);

            if(couleur == 255 && col!=501)//L'emplacement de carte en bas est déjà utilisée (Si la l'emplacement actuel n'est pas en bas de l'écran <=> col=501)
            {
                couleur = getpixel(plateau, ligne+74/2, col+84);
                couleur = getr(couleur);
                if(couleur!=255)//Le chemin n'est pas connecté
                    placer=0;
            }
        }
        else///Pas de chemin en bas
        {
            couleur = getpixel(plateau, ligne+74/2, col+84);
            couleur = getr(couleur);
            if(couleur==255)//Un chemin existe sur la carte du bas : la carte ne peut pas être placée
                placer=0;
        }
    }

    if(nbOutils<3)   ///Le joueur n'a pas tous ses outils
        placer=0;

    couleur = getpixel(plateau, ligne+74/2, col+84/2);
    couleur = getg(couleur);
    if(couleur==255)///L'emplacement actuel est déjà utilisé
        placer=0;

    if(placer==1)//Le placement est possible
        rect(buffer, ligne, col, ligne+74, col+84, makecol(100,250, 100));
    if(placer==0) //le placement est impossible
    {
        rect(buffer, ligne, col, ligne+74, col+84, makecol(255,0, 0));
        if(*dragDrop==2)//Carte lachée
            *dragDrop=0;//Drag and drop annulé
    }
}


void connectionCarte(BITMAP *plateau, int ligne, int col, int *placer, int carte)
{
    int couleur, placer2=0;

    couleur = getpixel(plateau, ligne+74/2+74, col+84/2);///Chercher une carte à droite
    couleur = getg(couleur);
    if(couleur==255)
    {
        couleur = getpixel(plateau, ligne+74, col+84/2);
        couleur = getr(couleur);
        if(couleur==255 && carte>=1 && carte<=4)//Les chemins se connectent
            placer2=1;
    }
    couleur = getpixel(plateau, ligne+74/2-74, col+84/2);///Chercher une carte à gauche
    couleur = getg(couleur);
    if(couleur==255)
    {
        couleur = getpixel(plateau, ligne-1, col+84/2);
        couleur = getr(couleur);
        if(couleur==255 && (carte==1 || carte==2 || carte==4 || carte==5))//Les chemins se connectent
            placer2=1;
    }
    couleur = getpixel(plateau, ligne+74/2, col+84/2+84);///Chercher une carte en bas
    couleur = getg(couleur);
    if(couleur==255)
    {
        couleur = getpixel(plateau, ligne+74/2, col+84);
        couleur = getr(couleur);
        if(couleur==255 && (carte==3 || carte==4))//Les chemins se connectent
            placer2=1;
    }
    couleur = getpixel(plateau, ligne+74/2, col+84/2-84);///Chercher une carte en haut
    couleur = getg(couleur);
    if(couleur==255)
    {
        couleur = getpixel(plateau, ligne+74/2, col-1);
        couleur = getr(couleur);
        if(couleur==255 && (carte==2 || carte==3 || carte==4))//Les chemins se connectent
            placer2=1;
    }

    if(placer2==0)//L'emplacement ne contient aucune carte adjacente
        *placer=0;
}


void eboulement(int *dragDrop, BITMAP *buffer, BITMAP *plateau)
{
    int i, j, placer=0, ligne=-100, col=-100, couleur;

    ///Aligner la carte sur le plateau
    for(i=0; i<7; i++)
    {
        for(j=0; j<5; j++)
        {
            if(mouse_x>=180+i*74 && mouse_x<=180+(i+1)*74 && mouse_y>=165+j*84 && mouse_y<=165+(j+1)*84)//Divise le plateau en 35 rectangles, trouve sur le quel se situe la souris
            {
                ligne=180 + i*74;
                col=165 + j*84;
            }
        }
    }
    couleur = getpixel(plateau, ligne+74/2, col+84/2);
    couleur = getg(couleur);
    if(couleur==255)//Il y a une carte à l'emplacement actuel
        placer=1;


    if(placer==1)//Le placement est possible
        rect(buffer, ligne, col, ligne+74, col+84, makecol(100,250, 100));
    if(placer==0 && mouse_y>=180  && mouse_x>190 && mouse_x<700) //le placement est impossible
    {
        rect(buffer, ligne, col, ligne+74, col+84, makecol(255,0, 0));
        if(*dragDrop==2)//Carte lachée
            *dragDrop=0;//Drag and drop annulé
    }
}

void detruireGalerie(BITMAP *plateau)
{
    int i, j, lig=-100, col=-100;

    ///trouver la bonne carte
    for(i=0; i<7; i++)
    {
        for(j=0; j<5; j++)
        {
            if(mouse_x>=180+i*74 && mouse_x<=180+(i+1)*74 && mouse_y>=165+j*84 && mouse_y<=165+(j+1)*84)//Divise le plateau en 35 rectangles, trouve sur le quel se situe la souris
            {
                lig=180 + i*74;
                col=165 + j*84;
            }
        }
    }
    for(i=0; i<74; i++)
    {
        for(j=0; j<84; j++)
            putpixel(plateau, i+lig, j+col, makecol(0, 0, 0));
    }
}



void afficherMessage(char message[40])
{
    int touche=0;

    BITMAP *bufferTemp;
    bufferTemp = create_bitmap(300, 200);
    BITMAP *fond;
    fond = load_bitmap("confirmer/fond.bmp", NULL);
    ///Afficher: le message
    do
    {
        if(key[KEY_ESC])
            touche=1;
        stretch_sprite(bufferTemp, fond, 0,0, 300, 200);
        rect(bufferTemp, 0,0,299,199,makecol(0,0,0));
        textprintf_ex(bufferTemp, font, 30, 70, makecol(255, 255, 255), -1, "%s", message);
        blit(bufferTemp, screen, -250,-200,0,0, SCREEN_W, SCREEN_H);
    }
    while(key[KEY_ESC] || touche==0);
    destroy_bitmap(bufferTemp);
    destroy_bitmap(fond);
}

