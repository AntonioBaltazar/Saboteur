#include "header.h"

void Sauvegarder(t_joueur joueur[8], int nbJoueurs, int tour, int pepite[3], int ordi, BITMAP *plateau)
{
    int i, j;

    FILE* fichier = NULL;
    fichier = fopen("sauv.txt", "w+");

    //Retirer le retour ligne de la défausse:
    //defausse[strlen(defausse)-1] = 0;

    ///Général
    for(i=0; i<3; i++)
        fprintf(fichier, "pepite:     %d\n", pepite[i]);
    fprintf(fichier, "Tour:       %d\n", tour);
    fprintf(fichier, "ordi:       %d\n", ordi);
    fprintf(fichier, "nbJoueurs:  %d\n", nbJoueurs);

    ///Joueurs
    for(i=0; i<nbJoueurs; i++)
    {
        fprintf(fichier, "role        %s\n", joueur[i].role);
        fprintf(fichier, "nom:        %s\n", joueur[i].pseudo);
        fprintf(fichier, "outil:      %d\n", joueur[i].outils[0]);
        fprintf(fichier, "outil:      %d\n", joueur[i].outils[1]);
        fprintf(fichier, "outil:      %d\n", joueur[i].outils[2]);
        fprintf(fichier, "score:      %d\n", joueur[i].score);
        for(j=0; j<7; j++)
            fprintf(fichier,"carte %d:    %s\n", j, joueur[i].cartes[j]);
        fprintf(fichier, "logo:       %d\n", joueur[i].logo);
    }

    ///Plateau
    PALETTE pal;

    get_palette(pal);
    save_bitmap("Autres/plateau.bmp", plateau, pal);

    fclose(fichier);
}

void SauvegarderPile(t_Pile *pile)
{
    FILE* fichier = NULL;
    fichier = fopen("pioche.txt", "w+");

    t_carte *carte = pile->premier;

    while(carte!=NULL)//Jusqu'à la fin de la pioche
    {
        fprintf(fichier, "carte:   %s\n", carte->valeur);
        carte = carte->suivant;
    }

    fclose(fichier);
    free(carte);
}



void ReprendreJeu()
{
    t_joueur joueur[8];
    t_Pile * pile = (t_Pile*)malloc(sizeof(t_Pile));
    pile->premier=NULL;
    int nbJoueurs, tour, pepite[3], ordi;
    char defausse[10];

    BITMAP *plateau;
    plateau = create_bitmap(SCREEN_W, SCREEN_H);

    t_image *nain;
    nain = (t_image*)malloc(sizeof(t_image)*8);
    initNains(nain);

    Reprendre(joueur, &nbJoueurs, &tour, pepite, defausse, &ordi);

    ///Plateau
    plateau = load_bitmap("Autres/plateau.bmp", NULL);
    if(plateau==NULL)
    {
        allegro_message("Il n'y a pas de partie sauvegardee");
    }

    ///ReprendrePile(pile):
    FILE* fichier = NULL;
    fichier = fopen("pioche.txt", "r");
    char valCarte[30];

    do
    {
        fseek(fichier, 9, SEEK_CUR);
        fgets(valCarte,30, fichier);
        if(valCarte[3]=='1' || valCarte[3]=='0')//Retirer les retours à la ligne
            valCarte[4] = 0;
        else
            valCarte[3] = 0;

        ajouterCarte(pile, valCarte);
    }
    while(!feof(fichier));

    fclose(fichier);

    ///Lancer le jeu
    MainPlateau(nbJoueurs, nain, joueur, pile, plateau, pepite, defausse, ordi, tour);

    destroy_bitmap(plateau);
}

void Reprendre(t_joueur joueur[8], int *nbJoueurs, int *tour, int pepite[3], char defausse[10], int *ordi)
{
    int i, j;

    FILE* fichier = NULL;
    fichier = fopen("sauv.txt", "r");

    if(fichier==NULL)
    {
        allegro_message("Il n'y a pas de partie sauvegardee");
    }
    ///Général
    for(i=0; i<3; i++)
    {
        fseek(fichier, 12, SEEK_CUR);
        fscanf(fichier, "%d\n", &pepite[i]);
    }
    fseek(fichier, 12, SEEK_CUR);
    fscanf(fichier, "%d\n", tour);
    fseek(fichier, 12, SEEK_CUR);
    fscanf(fichier, "%d\n", ordi);
    fseek(fichier, 12, SEEK_CUR);
    fscanf(fichier, "%d\n", nbJoueurs);

    ///Joueurs
    for(i=0; i<*nbJoueurs; i++)
    {
        fseek(fichier, 12, SEEK_CUR);
        fscanf(fichier, " %s\n", joueur[i].role);
        fseek(fichier, 12, SEEK_CUR);
        fscanf(fichier, "%s\n", joueur[i].pseudo);
        fseek(fichier, 12, SEEK_CUR);
        fscanf(fichier, "%d\n", &joueur[i].outils[0]);
        fseek(fichier, 12, SEEK_CUR);
        fscanf(fichier, "%d\n", &joueur[i].outils[1]);
        fseek(fichier, 12, SEEK_CUR);
        fscanf(fichier, "%d\n", &joueur[i].outils[2]);
        fseek(fichier, 12, SEEK_CUR);
        fscanf(fichier, "%d\n", &joueur[i].score);
        for(j=0; j<7; j++)
        {
            fseek(fichier, 12, SEEK_CUR);
            fgets(joueur[i].cartes[j], 10, fichier);
            joueur[i].cartes[j][strlen(joueur[i].cartes[j])-1] = 0;
        }
        fseek(fichier, 12, SEEK_CUR);
        fscanf(fichier, "%d\n", &joueur[i].logo);
    }

    sprintf(defausse, "0");

    fclose(fichier);
}


void ReprendrePile(t_Pile * pile)
{
    FILE* fichier = NULL;
    fichier = fopen("pioche.txt", "r");
    char valCarte[30];

    do
    {
        fseek(fichier, 9, SEEK_CUR);
        fgets(valCarte,30, fichier);
        if(valCarte[3]=='1' || valCarte[3]=='0')//Retirer les retours à la ligne
            valCarte[4] = 0;
        else
            valCarte[3] = 0;

        ajouterCarte(pile, valCarte);
    }
    while(!feof(fichier));

    fclose(fichier);
}


void ajouterCarte(t_Pile *pile, char valCarte[30])
{
    t_carte *nouveau =(t_carte*)malloc(sizeof(t_carte));
    if (pile == NULL || nouveau == NULL)
    {
        allegro_message("erreur de pioche");
        exit(EXIT_FAILURE);
    }

    strcpy(nouveau->valeur, valCarte);
    nouveau->suivant = pile->premier;
    pile->premier = nouveau;
}


void sauvegarde_score(t_joueur joueur[8], int nb_joueur)
{
    ///Déclaration des variables + ouverture du fichier et son blindage
    FILE* fichier = NULL;
    fichier = fopen("sauvegarde_score.txt","w");
    int i = 0;

    if(fichier == NULL)
    {
        allegro_message("Erreur : Le fichier sauvegarde n'a pas ete correctement ouvert !\n");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    else
    {
        fseek(fichier,0,SEEK_END);
        for(i=0; i<nb_joueur; i++)
        {
            fprintf(fichier,"Nom:   %s\n",joueur[i].pseudo); //Sauvegarde du pseudo
            fprintf(fichier,"score: %d\n",joueur[i].score);   //Sauvegarde du score
            fprintf(fichier,"logo:  %d\n",joueur[i].logo);   //Sauvegarde du score
        }
    }
}
