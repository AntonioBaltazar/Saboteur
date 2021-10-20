#include "header.h"

void initialiserJeu(t_carte *tab)
{

    int i=0;

    for(i=0; i<1; i++)
    {
        strcpy(tab[i].valeur,"A 10");
    }
    for (i=1; i<10; i++)
    {
        strcpy(tab[i].valeur,"G 2");
    }
    for (i=10; i<19; i++)
    {
        strcpy(tab[i].valeur,"G 3");
    }
    for (i=19; i<28; i++)
    {
        strcpy(tab[i].valeur,"G 5");
    }
    for (i=28; i<37; i++)
    {
        strcpy(tab[i].valeur,"G 4");
    }
    for (i=37; i<46; i++)
    {
        strcpy(tab[i].valeur,"G 1");
    }
    for (i=46; i<48; i++)
    {
        strcpy(tab[i].valeur,"A 1");
    }
    for (i=48; i<50; i++)
    {
        strcpy(tab[i].valeur,"A 7");
    }
    for (i=50; i<52; i++)
    {
        strcpy(tab[i].valeur,"A 2");
    }
    for (i=52; i<54; i++)
    {
        strcpy(tab[i].valeur,"A 8");
    }
    for (i=54; i<56; i++)
    {
        strcpy(tab[i].valeur,"A 0");
    }
    for (i=56; i<58; i++)
    {
        strcpy(tab[i].valeur,"A 6");
    }
    for (i=58; i<60; i++)
    {
        strcpy(tab[i].valeur,"A 11");
    }
    for (i=60; i<62; i++)
    {
        strcpy(tab[i].valeur,"A 9");
    }
    for (i=62; i<64; i++)
    {
        strcpy(tab[i].valeur,"A 5");
    }
    for (i=64; i<66; i++)
    {
        strcpy(tab[i].valeur,"A 3");
    }
    for (i=66; i<68; i++)
    {
        strcpy(tab[i].valeur,"A 4");
    }
    for (i=68; i<70; i++)
    {
        strcpy(tab[i].valeur,"A 10");
    }
}

void melangerJeu(t_carte *tab, t_carte *tabmelangee)
{

    int i,j;
    char tmp[30];

    for(i=0; i<70; i++)
    {
        strcpy(tmp,tab[i].valeur);
        j=rand()%70;
        strcpy(tabmelangee[i].valeur,tab[j].valeur);
        strcpy(tab[j].valeur,tmp);
    }
    /* for(i=0; i<70; i++)
     {
         printf("%s\n",tabmelangee[i].valeur);
     }*/
}

void creationPioche(t_Pile * pile,t_carte *tabmelangee)
{
    //enpiler les cartes du tabmelanger
    int i=0;
    pile->premier=NULL;

    for (i=0; i<70; i++)
    {
        t_carte *nouveau = (t_carte*)malloc(sizeof(t_carte));

        if (pile == NULL || nouveau == NULL)
        {
            printf("Probleme");
        }
        else
        {
            strcpy(nouveau->valeur,tabmelangee[i].valeur);
            //printf("%s\n",nouveau->valeur);
            nouveau->suivant = pile->premier;
            pile->premier = nouveau;
        }
    }
}

char* piocher(t_Pile * pile)
{
    if(pile==NULL)
    {
        printf("Probleme");
    }
    char * valeurDepilee;
    valeurDepilee = (char*)malloc(sizeof(char)*30);

    t_carte *aDepiler = pile->premier;

    if (pile != NULL && pile->premier != NULL)
    {
        strcpy(valeurDepilee,aDepiler->valeur);
        pile->premier =aDepiler->suivant;
        free(aDepiler);
    }

    return valeurDepilee;
}

