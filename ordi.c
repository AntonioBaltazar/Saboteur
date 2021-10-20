#include "header.h"

void TourOrdi(t_joueur joueur[8], t_Pile *pile, int nbJoueurs, BITMAP *plateau, char defausse[10])    //initialise les variables des joueurs
{
    int decision=0, typeDecision=0;//Déterminer l'importance de chaque action. La plus forte sera jouée
    int detruire=0, casserOutil=0, reparerOutil=0, galerie=0, defausser=1;//Liste des actions que l'ia pourrait effectuer

    int i, nbCartes=0;

    for(i=0; i<7; i++)
    {
        if(strcmp(joueur[1].cartes[i], "0")!=0)//La carte existe
            nbCartes++;
    }

    ///Piocher (si possible)
    if(nbCartes<7 && pile->premier!=NULL)
    {
        for(i=0; i<7; i++)
        {
            if(strcmp(joueur[1].cartes[i], "0")==0)
                strcpy(joueur[1].cartes[i], piocher(pile));
        }
        defausser=0;
    }

    ///Déterminer les actions possibles (c'est dans le nom)
    determinerActionsPossibles(joueur[1], &detruire, &casserOutil, &reparerOutil, &galerie, &defausser);

    ///Calculer la meilleure des actions possibles
    if(detruire==1)
        detruireCarte(&joueur[1], &decision, &typeDecision, plateau, defausse, 0);

    if(reparerOutil==1)
        reparer(joueur, &decision, &typeDecision, defausse, 0);

    if(casserOutil==1)
        casser(joueur, &decision, &typeDecision, nbJoueurs, defausse, 0);

    if(galerie==1)
        creuser(&joueur[1], &decision, &typeDecision, plateau, 0);

    if(defausser==1 && decision==0 && pile->premier!=NULL) //Aucune action n'est réalisable(sauf piocher)
    {
        typeDecision=5;
    }


    ///Agir
    if(typeDecision==1)///Détruire
        detruireCarte(&joueur[1], &decision, &typeDecision, plateau, defausse, 1);
    if(typeDecision==2)///casser outil
        casser(joueur, &decision, &typeDecision, nbJoueurs, defausse, 1);
    if(typeDecision==3)///réparer
        reparer(joueur, &decision, &typeDecision, defausse, 1);
    if(typeDecision==4)///creuser
        creuser(&joueur[1], &decision, &typeDecision, plateau, 1);
    if(typeDecision==5)///defausser
        defausserCarte(&joueur[1], defausse);

    if(typeDecision==0)//Rien n'est possible
    {
        /*Rien ne se passe ici.
        L'IA passe donc son tour*/
    }

    ///Afficher un résumé
    Resume(typeDecision, defausse);
}


void determinerActionsPossibles(t_joueur joueur, int *detruire, int *casserOutil, int *reparerOutil, int *creuser, int *defausser)
{
    int i;

    for(i=0; i<7; i++)
    {
        if(strcmp(joueur.cartes[i], "0")!=0)//La carte existe
        {
            if(joueur.cartes[i][2]-48>=6 && joueur.cartes[i][2]-48<=9 && joueur.cartes[i][0]=='A')//Carte détruire outil
                *casserOutil=1;
            if(joueur.cartes[i][2]-48>=0 && joueur.cartes[i][2]-48<=5 && joueur.cartes[i][0]=='A')//Carte réparer outil
                *reparerOutil=1;
            if(strcmp(joueur.cartes[i], "A 10")==0)//Carte détruire chemin
                *detruire=1;
            if(joueur.cartes[i][0]=='G')//Carte creuser chemin
                *creuser=1;
        }
    }
}


void detruireCarte(t_joueur *IA, int *decision, int *typeDecision, BITMAP *plateau, char defausse[10], int agir)
{
    int i, j, ligne, col, couleur, numCarte=0;
    int importance;

    for(i=0; i<7; i++)
    {
        if(strcmp(IA->cartes[i],"A 10")==0)
        {
            numCarte=i;
            i=7;
        }
    }

    for(i=0; i<7; i++)///Pour chaque emplacement de carte du plateau
    {
        for(j=0; j<5; j++)
        {
            ligne=180 + i*74;
            col=165 + j*84;

            couleur = getpixel(plateau, ligne+74/2, col+84/2);
            couleur = getg(couleur);

            if(couleur == 255)//L'emplacement de carte actuelle est déjà utilisée
            {
                importance = 5 + i;   //De 5 à 9 en importance

                if(importance>*decision)
                {
                    *decision = 5 + i;
                    *typeDecision=1;
                }
                if(importance==*decision)//Carte trouvée: sortir des boucles
                {
                    i=7;
                    j=5;
                }
            }
        }
    }

    if(agir==1)///Détruire la carte demandée
    {
        for(i=0; i<74; i++)
        {
            for(j=0; j<84; j++)
                putpixel(plateau, i+ligne, j+col, makecol(0, 0, 0));
        }
        sprintf(defausse, "A 10");
        sprintf(IA->cartes[numCarte], "0");
    }
}


void casser(t_joueur joueur[8], int *decision, int *typeDecision, int nbJoueurs, char defausse[10], int agir)
{
    int i, numCarte=0;
    int importance=0, nbOutils=0, pioche=0, lanterne=0, charette=0;

    for(i=0; i<7; i++)//Trouver le nb d'outils
    {
        if(joueur[1].cartes[i][0]=='A')
        {
            if(nbOutils!=3)
            {
                if(joueur[1].cartes[i][2]-48==6) //Lanterne
                {
                    nbOutils=1;
                    lanterne=1;
                    pioche=0;
                    charette=0;
                    numCarte = i;
                }
                if(joueur[1].cartes[i][2]-48==7) //charette
                {
                    nbOutils=1;
                    charette=1;
                    pioche=0;
                    lanterne=0;
                    numCarte = i;
                }
                if(joueur[1].cartes[i][2]-48==8) //pioche
                {
                    nbOutils=1;
                    pioche=1;
                    charette=0;
                    lanterne=0;
                    numCarte = i;
                }
                if(joueur[1].cartes[i][2]-48==9) //Tout casser
                {
                    nbOutils=3;
                    pioche=1;
                    charette=1;
                    lanterne=1;
                    numCarte = i;
                }
            }
        }
    }
    if(nbOutils>0)
        importance = 5+nbOutils;//Importance de 6 à 8

    ///Tester si la carte sera utile
    if(pioche==1)
    {
        if(joueur[0].outils[2]==1)//Pioche à casser
        {
            if(*decision<importance)
            {
                *decision=importance;
                *typeDecision=2;
            }
        }
    }
    if(lanterne==1)
    {
        if(joueur[0].outils[0]==1)//lanterne à casser
        {
            if(*decision<importance)
            {
                *decision=importance;
                *typeDecision=2;
            }
        }
    }
    if(charette==1)
    {
        if(joueur[0].outils[1]==1)//charette à casser
        {
            if(*decision<importance)
            {
                *decision=importance;
                *typeDecision=2;
            }
        }
    }

    ///Agir
    if(agir==1)
    {
        if(pioche==1)
            joueur[0].outils[2]=0;
        if(lanterne==1)
            joueur[0].outils[0]=0;
        if(charette==1)
            joueur[0].outils[1]=0;
        sprintf(defausse, joueur[1].cartes[numCarte]);
        sprintf(joueur[1].cartes[numCarte], "0");
    }
}


void reparer(t_joueur joueur[8], int *decision, int *typeDecision, char defausse[10], int agir)
{
    int i, numCarte=0;
    int importance=0, nbOutils=0, pioche=0, lanterne=0, charette=0;

    for(i=0; i<7; i++)//Trouver le nb d'outils
    {
        if(joueur[1].cartes[i][0]=='A' && joueur[1].cartes[i][3]!=0 && joueur[1].cartes[i][3]!=1)//Cartes action, pas A 10 ni A 11
        {
            if(nbOutils!=3)
            {
                if(joueur[1].cartes[i][2]-48==0) //Lanterne
                {
                    if(nbOutils<=1)
                    {
                        nbOutils=1;
                        lanterne=1;
                        pioche=0;
                        charette=0;
                        numCarte = i;
                    }
                }
                if(joueur[1].cartes[i][2]-48==1) //charette
                {
                    if(nbOutils<=1)
                    {
                        nbOutils=1;
                        lanterne=0;
                        pioche=0;
                        charette=1;
                        numCarte = i;
                    }
                }
                if(joueur[1].cartes[i][2]-48==2) //pioche
                {
                    if(nbOutils<=1)
                    {
                        nbOutils=1;
                        lanterne=0;
                        pioche=1;
                        charette=0;
                        numCarte = i;
                    }
                }
                if(joueur[1].cartes[i][2]-48==3) //pioche charette
                {
                    nbOutils=2;
                    pioche=1;
                    charette=1;
                    lanterne=0;
                    numCarte = i;
                }
                if(joueur[1].cartes[i][2]-48==4) //charette lanterne
                {
                    nbOutils=2;
                    pioche=0;
                    charette=1;
                    lanterne=1;
                    numCarte = i;
                }
                if(joueur[1].cartes[i][2]-48==5) //pioche lanterne
                {
                    nbOutils=2;
                    pioche=1;
                    charette=0;
                    lanterne=1;
                    numCarte = i;
                }
            }
        }
    }
    if(nbOutils>0)
        importance = 3+nbOutils;//Importance de 4 à 5


    if(*decision<importance)
    {
        if((pioche==1 && joueur[1].outils[2]==0) || (lanterne==1 && joueur[1].outils[0]==0) || (charette==1 && joueur[1].outils[1]==0))//La carte aura un effet
        {
            *decision=importance;
            *typeDecision=3;
        }
    }

    ///Agir
    if(agir==1)
    {
        if(pioche==1)
            joueur[1].outils[2]=1;
        if(lanterne==1)
            joueur[1].outils[0]=1;
        if(charette==1)
            joueur[1].outils[1]=1;
        sprintf(defausse, joueur[1].cartes[numCarte]);
        sprintf(joueur[1].cartes[numCarte], "0");
    }
}


void creuser(t_joueur *IA, int *decision, int *typeDecision, BITMAP *plateau, int agir)
{
    int i, j, ligne, col;
    int impCarte=0, numCarte=-1, importance=0, possible=0;

    ///Chercher la meilleure carte à jouer, entre: chemin tout droit/Impasse
    for(i=0; i<7; i++)
    {
        if(IA->cartes[i][0]=='G')
        {
            if(IA->cartes[i][2]-48==1 && impCarte==0)//Carte chemin tout droit
            {
                impCarte=1;
                numCarte=i;
            }
            if(IA->cartes[i][2]-48==5)//Carte impasse (la meilleure)
            {
                impCarte=4;
                numCarte=i;
                i=7;//arrêt des recherches
            }

        }
    }
    if(impCarte!=0)
        importance=2+impCarte;  //Importance de 3 à 6

    if(numCarte!=-1)//Carte utile en possession: chemin droit ou impasse
    {
        ///Tenter de jouer la carte:
        for(i=0; i<7; i++)
        {
            for(j=0; j<5; j++)
            {
                ligne=180 + i*74;
                col=165 + j*84;

                possible = verifierPlateauOrdi(IA->outils[0]+IA->outils[1]+IA->outils[2], IA->cartes[numCarte][2]-48, plateau, ligne, col);
                if(possible==1)//La carte peut être posée?
                {
                    if(*decision<importance)
                    {
                        *decision = importance;
                        *typeDecision = 4;
                    }
                    if(agir==1)///Agir
                    {
                        if(IA->cartes[numCarte][2]-48==5)//Carte impasse jouée
                        {
                            putpixel(plateau, ligne, col+84/2, makecol(255, 0, 0));
                            for(i=0; i<5; i++)
                                putpixel(plateau, ligne+74/2+i, col+84/2, makecol(0, 255, 0)); //Indique le type de carte
                            sprintf(IA->cartes[numCarte], "0");
                        }
                        if(IA->cartes[numCarte][2]-48==1)//Carte chemin droit joué
                        {
                            putpixel(plateau, ligne+73, col+84/2, makecol(255, 0, 0));
                            putpixel(plateau, ligne, col+84/2, makecol(255, 0, 0));
                            putpixel(plateau, ligne+74/2, col+84/2, makecol(0, 255, 0)); //Indique le type de carte
                            sprintf(IA->cartes[numCarte], "0");
                        }
                    }
                    i=7;
                    j=5;
                }
            }
        }
    }
}


void defausserCarte(t_joueur *IA, char defausse[10])
{
    int i, aleat;

    for(i=0; i<3; i++)
    {
        do
        {
            aleat = rand()% (6-0+1)+0;//De 6 à 0
        }
        while(strcmp(IA->cartes[aleat],"0")==0);
        sprintf(defausse, IA->cartes[aleat]);
        sprintf(IA->cartes[aleat],"0");
    }
}


int verifierPlateauOrdi(int nbOutils, int carte, BITMAP *plateau, int ligne, int col)
{
    int couleur, placer=1;

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

    if(nbOutils<3)   ///L'IA n'a pas tous ses outils
        placer=0;

    couleur = getpixel(plateau, ligne+74/2, col+84/2);
    couleur = getg(couleur);
    if(couleur==255)///L'emplacement actuel est déjà utilisé
        placer=0;

    return placer;
}


void Resume(int type, int defausse)
{
    char message[30];

    ///Définir le message
    if(type==0)
        sprintf(message, "Passe son tour");
    if(type==1)
        sprintf(message, "Detruire une galerie");
    if(type==2)
        sprintf(message, "Casser des outils");
    if(type==3)
        sprintf(message, "Reparer ses outils");
    if(type==4)
        sprintf(message, "Creuser une galerie");
    if(type==5)
        sprintf(message, "Defausser 3 cartes");

    allegro_message("L'IA joue: %s", message);
}
