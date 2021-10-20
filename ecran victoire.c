#include "header.h"

///Affichage d'une animation lorsque les chercheurs ont gagnés la partie
void victoire_chercheur()
{
    ///Déclarations des variables utilisées
    BITMAP **frames = NULL;
    int *durations = NULL;
    char nom_fichier[30];
    int i = 0;
    int nb_image = 0;
    int escape=0;

    ///Création d'un buffer
    BITMAP *buffer = NULL;
    buffer = create_bitmap(800, 600);

    if(buffer == NULL)
    {
        allegro_message("Erreur de declaration de bitmap de l'ecran de victoire des chercheurs");
        set_gfx_mode(GFX_TEXT,0,0,800,600);
    }

    ///Téléchargement du gif
    sprintf(nom_fichier,"ecran_victoire/chercheur.gif");
    nb_image = algif_load_animation (nom_fichier, &frames, &durations);

    ///Blindage sur le bon téléchargement du gif
    if(nb_image)
    {
        ///Tant que l'utilisateur n'a pas cliquer sur la touche escape on affiche l'écran de victoire
        do
        {
            ///Affichage de chacune des images qui composent le gif
            for (i = 0; i < nb_image && !escape; i++)
            {
                ///Si l'utilisateur clique sur la touche escape on met la variable à 1 pour quitter le sous programme
                if(key[KEY_ESC])
                    escape = 1;

                ///Affichage du gif
                stretch_sprite(buffer,frames[i],0,0,800,600);
                blit(buffer,screen,0,0,0,0,800,600);

                rest(30);
            }
        }
        while(!escape);

        ///Libération de l'espace mémoire utilisé
        for (i = 0; i < nb_image; i++)
            destroy_bitmap (frames[i]);
        free(frames);
        free(durations);
        destroy_bitmap(buffer);
    }
    else
        ///Le téléchargement du gif a échoué
        allegro_message ("Erreur : le gif <video_gif> n'a pas ete correctement télecharger");
}


///Affichage d'une animation lorsque tout le monde a perdu
void game_over()
{
    ///Déclarations des variables utilisées
    BITMAP **frames = NULL;
    int *durations = NULL;
    char nom_fichier[30];
    int i = 0;
    int nb_image = 0;
    int escape=0;

    ///Création d'un buffer
    BITMAP *buffer = NULL;
    buffer = create_bitmap(800, 600);

    if(buffer == NULL)
    {
        allegro_message("Erreur de declaration de bitmap de l'ecran de victoire des chercheurs");
        set_gfx_mode(GFX_TEXT,0,0,800,600);
    }

    ///Téléchargement du gif
    sprintf(nom_fichier,"ecran_victoire/game_over.gif");
    nb_image = algif_load_animation (nom_fichier, &frames, &durations);

    ///Blindage sur le bon téléchargement du gif
    if(nb_image)
    {
        ///Tant que l'utilisateur n'a pas cliquer sur la touche escape on affiche l'écran de victoire
        do
        {

            for (i = 0; i < nb_image && !escape; i++)
            {
                ///Si l'utilisateur clique sur la touche escape on met la variable à 1 pour quitter le sous programme
                if(key[KEY_ENTER])
                    escape = 1;

                ///Affichage du gif
                draw_sprite(buffer,frames[i],150,50);
                rectfill(buffer,500,500,800,600,makecol(0,0,0));
                blit(buffer,screen,0,0,0,0,800,600);

                rest(70);
            }
        }
        while(!escape);

        ///Libération de l'espace mémoire utilisé
        for (i = 0; i < nb_image; i++)
            destroy_bitmap (frames[i]);
        free(frames);
        free(durations);
        destroy_bitmap(buffer);
    }
    else
        ///Le téléchargement du gif a échoué
        allegro_message ("Erreur : le gif <video_gif> n'a pas ete correctement télecharger");
}





///    ------------------   J'étais entrain d'essayer d'eviter la lecture sauvegarde donc j'appelle victoire saboteur dans nouvelle partie et je ne comprend pas pourquoi ca crash    ---------------------------------------  ////


///Affichage d'une animation lorsque les saboteurs ont gagnés la partie
void victoire_saboteur(t_joueur joueur[8], int nb_joueur)
{
    ///Déclaration
    int i = 0, saboteur[2], chercheur[6], nb_saboteur = 0,nb_chercheur = 0, j = 4,k=0;
    int x1 = 20, y1 = 300, taille_nainx = 50, taille_nainy = 80, x2=20;
    float  y2=300,x=50, sinus;

    int quit = 0;
    t_image* pierre=NULL;
    t_image* nain = NULL;
    t_image* fond = NULL;
    BITMAP* buffer;

    ///allocation dynamique + creation de bitmap
    buffer = create_bitmap(800,600);
    pierre = (t_image*)malloc(sizeof(t_image)*7);
    nain = (t_image*)malloc(sizeof(t_image)*8);
    fond = (t_image*)malloc(sizeof(t_image)*2);

    telechargement_victoire_saboteur(pierre,fond,nain);

    ///Blindage sur les tableau et bitmap
    if(pierre==NULL || buffer == NULL || nain == NULL || fond == NULL)
        allegro_message("Erreur de bitmap pour la victoire du saboteur");
    else
    {
        ///Détermination du nombre de saboteur et de chercheur
        for(i=0; i<nb_joueur; i++)
        {
            //Le joueur i est un saboteur, on note sa postion dans un tableau d'entier
            if(strcmp(joueur[i].role,"saboteur")==0)
            {
                saboteur[nb_saboteur]=i;
                nb_saboteur++;
            }

            //Le joueur i est un chercheur, on note sa position dans un tableau d'entier
            else
            {
                chercheur[nb_chercheur]=i;
                nb_chercheur++;
            }
        }

        ///Positionnement de départ aléatoire des chercheurs
        for(i=0; i<nb_chercheur; i++)
        {
            nain[chercheur[i]].pos_x = rand()%((800-taille_nainx)-400+1) + 400;
            nain[chercheur[i]].pos_y = rand()%((600-taille_nainy)-200+1) + 200;

            if(i%2 == 0)
            {
                nain[chercheur[i]].mouvementx = 1;
                nain[chercheur[i]].mouvementy = 1;
            }
            else
            {
                nain[chercheur[i]].mouvementx = 2;
                nain[chercheur[i]].mouvementy = 2;
            }
        }
        i=0;


        ///Initialisation de la position du fond d'écran
        fond[1].pos_x = 0;
        ///Affichage
        do
        {
            clear_bitmap(buffer);

            ///Controle du mouvement des nains
            mouvement_ecran_saboteur(&x1,&y1,nb_joueur-nb_saboteur,nain,chercheur,fond);

            ///Affichage du fond de la mine
            draw_sprite(buffer,fond[1].fichier,fond[1].pos_x,0);

            ///Affichage de "Victoire des Saboteurs"
            stretch_sprite(buffer,fond[0].fichier,200,80,400,60);

            ///Affichage des pierres
            stretch_sprite(buffer,pierre[i].fichier,x2,y2,20,20);
            stretch_sprite(buffer,pierre[j].fichier,x2+20,y2+20,20,20);


            ///Affichage des saboteurs
            if(nb_saboteur == 1)
            {
                stretch_sprite(buffer,nain[saboteur[0]].fichier,x1,y1,taille_nainx,taille_nainy);
            }
            else if(nb_saboteur==2)
            {
                stretch_sprite(buffer,nain[saboteur[0]].fichier,x1,y1,taille_nainx,taille_nainy);
                stretch_sprite(buffer,nain[saboteur[1]].fichier,x1+60,y1,taille_nainx,taille_nainy);
            }



///     --------------------------       AFFICHAGE DES CHERCHEURS ALEATOIREMENT GRACE AU SP MOUVEMENT()    --------------------------------     ///

            ///Affichage des chercheurs
            for(k=0; k<nb_chercheur; k++)
            {
                stretch_sprite(buffer,nain[chercheur[k]].fichier,nain[chercheur[k]].pos_x,nain[chercheur[k]].pos_y,taille_nainx,taille_nainy);
            }


            blit(buffer,screen,0,0,0,0,800,600);

            ///Test si l'on a appuyé sur la touche echap
            if(key[KEY_ESC])
                quit=1;

            i++;
            j++;

            ///Permet de remettre à 0 l'animation des pierres
            if(i==7)
                i=0;
            if(j==7)
                j=0;

            rest(50);

            x2=x2+10;
            x=x+35;
            sinus = sin(x/780);
            if(sinus<0)
                sinus=-sinus;
            y2=-150*sinus+ 400;


            printf("%f\n", sin(x/780));

            if(x2>800)
            {
                x2 = x1 + 100;
                y2 = y1 + 20;
            }

        }
        while(quit!=1);
    }

    free(pierre);
    free(fond);
    free(nain);
    destroy_bitmap(buffer);
}



void mouvement_ecran_saboteur(int *x1, int *y1, int nb_chercheur, t_image* nain, int chercheur[6], t_image* fond)
{
    int i = 0, j = 0, touche=0;

    ///Paramètre de la proportion des images des personnages
    int vitesse_nain = 10;
    int taille_nainx = 50;
    int taille_nainy = 80;

    ///Détection des demandes de déplacement des saboteurs
    if( key[KEY_RIGHT] )
        *x1+=vitesse_nain;
    else if( key[KEY_LEFT] )
        *x1-=vitesse_nain;

    if( key[KEY_DOWN] )
        *y1+=vitesse_nain;
    else if(key[KEY_UP])
        *y1-=vitesse_nain;


    ///Controle des mouvements des saboteurs
    if(*x1 < 0)
        *x1+=vitesse_nain;

    if(*x1 > (180+taille_nainx))
        *x1-=vitesse_nain;

    if(*y1 < 200)
        *y1+=vitesse_nain;

    if( *y1 > (550-taille_nainy))
        *y1-=vitesse_nain;

    if(fond[1].pos_x == -3450)
        fond[1].pos_x = 0;
    else
    {
        fond[1].pos_x -= 10;
    }



    for(i=0; i<nb_chercheur ; i++)
    {
        ///Controle des bords

        //Controle de la position en x
        if(nain[chercheur[i]].pos_x <= 300)
            nain[chercheur[i]].mouvementx = 1;

        if(nain[chercheur[i]].pos_x >= (800-taille_nainx))
            nain[chercheur[i]].mouvementx = 2;

        //Controle de la position en y
        if(nain[chercheur[i]].pos_y <= 200)
            nain[chercheur[i]].mouvementy = 1;

        if( nain[chercheur[i]].pos_y >= (550-taille_nainy))
            nain[chercheur[i]].mouvementy = 2;


        //Test sur chacun des chercheurs
        for(j=0; j<nb_chercheur; j++)
        {
            touche=0;
            //Cas de collision entre deux chercheurs
            if(nain[chercheur[i]].pos_x+taille_nainx >= nain[chercheur[j]].pos_x && nain[chercheur[i]].pos_x+taille_nainx <= (nain[chercheur[j]].pos_x + taille_nainx) && nain[chercheur[i]].pos_y >= nain[chercheur[j]].pos_y && nain[chercheur[i]].pos_y <= (nain[chercheur[j]].pos_y + taille_nainy) && i!=j)
                touche=1;
            if(nain[chercheur[i]].pos_x >= nain[chercheur[j]].pos_x && nain[chercheur[i]].pos_x <= (nain[chercheur[j]].pos_x + taille_nainx) && nain[chercheur[i]].pos_y+taille_nainy >= nain[chercheur[j]].pos_y && nain[chercheur[i]].pos_y+taille_nainy <= (nain[chercheur[j]].pos_y + taille_nainy) && i!=j)
                touche=1;

            if(touche==1)
            {
                if(nain[chercheur[j]].mouvementx == 2)
                    nain[chercheur[j]].mouvementx = 1;
                else
                    nain[chercheur[j]].mouvementx = 2;

                if(nain[chercheur[j]].mouvementy == 2)
                    nain[chercheur[j]].mouvementy = 1;
                else
                    nain[chercheur[j]].mouvementy = 2;
            }
        }

        ///Déplacement automatique

        if(nain[chercheur[i]].mouvementx == 2)
            nain[chercheur[i]].pos_x -= vitesse_nain;
        else
            nain[chercheur[i]].pos_x += vitesse_nain;

        if(nain[chercheur[i]].mouvementy == 2)
            nain[chercheur[i]].pos_y -= vitesse_nain;
        else
            nain[chercheur[i]].pos_y += vitesse_nain;
    }

    rest(20);
}


void telechargement_victoire_saboteur(t_image *pierre, t_image *fond, t_image *nain)
{
    int i = 0;

    ///Telechargement de l'animation de lancer de pierre
    for(i=0; i<7; i++)
    {
        sprintf(pierre[i].nomfichier,"ecran_victoire/pierre%d.bmp",i+1);
        pierre[i].fichier = load_bitmap(pierre[i].nomfichier,NULL);

        if(pierre[i].fichier == NULL)
        {
            allegro_message("Erreur de telechargement de pierre%d",i+1);
            allegro_exit();
        }
    }
    ///Telechargement des logos des nains
    initNains(nain);

    ///Teléchargement de l'image de fond
    sprintf(fond[1].nomfichier,"ecran_victoire/mine.bmp");
    fond[1].fichier = load_bitmap(fond[1].nomfichier,NULL);


    ///Telechargement du texte de fond
    sprintf(fond[0].nomfichier,"ecran_victoire/saboteur.bmp");
    fond[0].fichier = load_bitmap(fond[0].nomfichier,NULL);

}



