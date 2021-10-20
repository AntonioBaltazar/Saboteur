#include "header.h"

void FinDePartie(t_joueur joueur[8], int numJ, int nbJoueurs, int *sortie, BITMAP *plateau, int pepite[3], t_Pile *pile)
{
    int resultat=0;
    ///Trouver si un chemin se connecte à une carte pépite:
    parcourir(&resultat, 0, 2, plateau, 0);

    if(resultat>0)///Récupérer pépites
    {
        GagnerPepites(resultat, pepite, joueur, numJ, nbJoueurs);
        victoire_chercheur();
        *sortie=1;
        game_over();
    }
    if(resultat==0 && pile->premier==NULL)///Pioche vide
    {
        afficherMessage("Il n'y a plus de cartes");
        victoire_saboteur(joueur, nbJoueurs);
        *sortie=1;
        game_over();
    }


}


void GagnerPepites(int resultat, int pepite[3], t_joueur joueur[8], int numJ, int nbJoueurs)
{
    int ordre, i=1, j, numPepite=resultat-1;
    char message[40], mot[16];
    for(j=0; j<40; j++)
        message[j]=0;

    if(joueur[numJ].role[0]=='c')
    {
        ///Creer le message:
        strcat(message,joueur[numJ].pseudo);
        sprintf(mot, " gagne ");
        strcat(message, mot);
        sprintf(mot, "%d", pepite[numPepite]);
        strcat(message, mot);
        sprintf(mot, " pepites");
        strcat(message, mot);
        joueur[numJ].score += pepite[numPepite];
        afficherMessage(message);

        pepite[numPepite]=0;
        numPepite++;
        if(numPepite==3)
            numPepite=0;
    }

    do
    {
        ordre = numJ+i;
        if(ordre>=nbJoueurs)
            ordre=ordre-nbJoueurs;

        if(joueur[ordre].role[0]=='c' && ordre!=numJ)//Le joueur peut gagner des point
        {
            if(pepite[numPepite]!=0)//Le carte existe encore
            {
                ///Creer le message:
                for(j=0; j<40; j++)
                    message[j]=0;
                sprintf(mot, "%s", joueur[numJ].pseudo);
                strcat(message,mot);
                sprintf(mot, " gagne ");
                strcat(message, mot);
                sprintf(mot, "%d", pepite[numPepite]);
                strcat(message, mot);
                sprintf(mot, " pepites");
                strcat(message, mot);
                joueur[numJ].score += pepite[numPepite];
                afficherMessage(message);

                joueur[ordre].score += pepite[numPepite];
                pepite[numPepite]=0;
                numPepite++;
                if(numPepite==3)
                    numPepite=0;
            }
        }
        i++;
    }
    while(i<=nbJoueurs);
}

void parcourir(int *resultat, int valLigne, int valCol, BITMAP *plateau, int provenance)
{
    int ligne, col, couleur=0, droite=0, haut=0, bas=0;
    ligne=180 + valLigne*74;
    col=165 + valCol*84;

    couleur = getpixel(plateau, ligne+74/2, col+84/2);
    couleur = getg(couleur);
    if(couleur==255)///La carte existe:
    {
        couleur = getpixel(plateau, ligne+73, col+84/2);
        droite = getr(couleur);
        couleur = getpixel(plateau, ligne+74/2, col);
        haut = getr(couleur);
        couleur = getpixel(plateau, ligne+74/2, col+83);
        bas = getr(couleur);

        if(droite==255)///Chemin Droite
        {
            valLigne+=1;
            parcourir(resultat, valLigne, valCol, plateau, 0);
            valLigne-=1;
        }
        if(bas==255 && provenance!=1)///Chemin bas
        {
            valCol+=1;
            parcourir(resultat, valLigne, valCol, plateau, 2);
            valCol-=1;
        }
        if(haut==255 && provenance!=2)///Chemin haut
        {
            valCol-=1;
            parcourir(resultat, valLigne, valCol, plateau, 1);
            valCol+=1;
        }
    }

    if(valLigne==6 && droite==255)//La carte tout à droite mène à droite = aux pépites
    {
        if(valCol==2)//Pépite milieu
        {
            *resultat=2;
        }
        if(valCol==0)//Pépite haut
        {
            *resultat=1;
        }
        if(valCol==4)//Pépite bas
        {
            *resultat=3;
        }
    }

}
