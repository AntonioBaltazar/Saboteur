#include "header.h"

//Gère le lancement du menu avec l'initialisation allegro
void menu()
{
    srand(time(NULL));
    initialisation_allegro(); //Initialisation allegro

    SAMPLE *music = NULL;               //Musique du menu
    music = load_sample("musique.wav");
    if(music==NULL)
        allegro_message("Erreur chargement: music.wav");

    t_image *elementA;                   //Déclaration d'un tableau d'image
    elementA = (t_image*)malloc(sizeof(t_image)*NB_ELEMENTA);
    int action = 0;

    ///Blindage sur l'allocation dynamique
    if(elementA==NULL)
    {
        allegro_message("Erreur : probleme d'allocation dynamique, le programme va se fermer automatiquement");
        exit(EXIT_FAILURE);
    }
    else
    {
        //Télecharge les images qui apparaissent sur le fond principal
        initialisation_image_menu_principal(elementA);

        do
        {
            play_sample(music,25,128,1000,1);

            //Lance le menu principal et donc le jeu
            action = menu_principal(elementA);

            switch(action)
            {
            case 1:
                stop_sample(music);
                nouvelle_partie();
                break;

            case 2:
                stop_sample(music);
                ReprendreJeu();
                break;

            case 3:
                stop_sample(music);
                aide();
                break;
            case 5:
                stop_sample(music);
                meilleur_joueur();
                break;
            }

        }
        while(action != 4);


        //Fermeture du mode graphique du mode graphique
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);

        //Libération du tableau d'image
        liberation_element(elementA,1);
        destroy_sample(music);
    }
}

//Initialisation allegro, mode graphique, clavier, souris et couleur
void initialisation_allegro()
{
    //Initialisation allegro
    allegro_init();

    //Permet d'initialiser la fonction de récupération de gif
    algif_init ();

    //Initialisation des couleurs
    set_color_depth(desktop_color_depth());

    //Installation du clavier
    install_keyboard();

    //Installation de la souris
    install_mouse();

    //Ouverture de la fenêtre graphique + blindage
    if((set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0))!=0)
    {
        allegro_message("Pb graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    //Installation du son
    if( install_sound(DIGI_AUTODETECT,MIDI_NONE,NULL)!= 0 )
    {
        allegro_message("Erreur d'installation du son");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}

//Initialisation des images cad taille, telechargement de l'image etc
void initialisation_image_menu_principal( t_image *element)
{
    int i;
    int pos_x=250;
    int taille_x = 550;
    int pos_y = 250;
    int taille_y = 280;

    ///Fond d'écran
    sprintf(element[8].nomfichier,"element/fond.bmp");         //Ecriture de l'adresse de l'image dans nomfichier
    element[10].fichier = load_bitmap(element[8].nomfichier,NULL);           //Téléchargement de l'image
    element[10].pos_x = 0;
    element[10].pos_y = 0;
    element[10].taille_x = 800;
    element[10].taille_y = 600;
    if(!element[10].fichier)                                       //Blindage sur le téléchargement du fond
        allegro_message("Erreur : L'image <fond> n'a pas ete correctement telecharger !");

    ///Ecriture de l'adresse de l'image dans nomfichier
    sprintf(element[0].nomfichier,"element/nouvelle partie.bmp");
    sprintf(element[1].nomfichier,"element/nouvelle partie2.bmp");
    sprintf(element[2].nomfichier,"element/reprendre.bmp");
    sprintf(element[3].nomfichier,"element/reprendre2.bmp");
    sprintf(element[4].nomfichier,"element/aide.bmp");
    sprintf(element[5].nomfichier,"element/aide2.bmp");
    sprintf(element[6].nomfichier,"element/score.bmp");
    sprintf(element[7].nomfichier,"element/score2.bmp");
    sprintf(element[8].nomfichier,"element/quitter.bmp");
    sprintf(element[9].nomfichier,"element/quitter2.bmp");


    for(i=0; i<10; i++)
    {
        ///Element non selectionné
        element[i].fichier = load_bitmap(element[i].nomfichier, NULL);     //Téléchargement de l'image
        element[i].pos_x = pos_x;
        element[i].pos_y = pos_y;
        element[i].taille_x = taille_x;
        element[i].taille_y = taille_y;

        if(!element[i].fichier)                                  //Blindage sur le téléchargement de l'image
            allegro_message("Erreur : L'image <%s> n'a pas ete correctement telecharger !",element[i].nomfichier);

        if(i%2 != 0)
        {
            pos_y +=50;
            taille_y = pos_y+30;
        }
    }
}


//Affiche le menu principal renvoie un chiffre permettant de lancer les fonctionnalités désirées
int menu_principal(t_image *elementA)
{
    //Creer le buffer, une zone sur laquelle serontt affichées toutes les images
    BITMAP *buffer;
    buffer = create_bitmap(800, 600);

    BITMAP *souris;                     //La souris
    souris = load_bitmap("Autres/souris.bmp", NULL);

    int choix = 0; //Permet de diriger les futures actions

    do
    {
        //Met un fond sur le buffer
        blit(elementA[10].fichier, buffer, elementA[10].pos_x, elementA[10].pos_y, 0, 0, elementA[10].taille_x, elementA[10].taille_y);

        ///La souris est sur la case nouvelle partie donc on l'affiche avec un effet de sélection
        if(mouse_x>=elementA[0].pos_x && mouse_x<=elementA[0].taille_x && mouse_y>=elementA[0].pos_x && mouse_y<=elementA[0].taille_y)
        {
            //affiche l'élément "nouvelle partie" sélectionné
            draw_sprite(buffer, elementA[1].fichier, elementA[1].pos_x, elementA[1].pos_y);
            if(mouse_b&1)
                choix = 1;
        }
        else
            //La souris n'est pas sur la case nouvelle partie donc on l'affiche normalement
            draw_sprite(buffer, elementA[0].fichier, elementA[0].pos_x, elementA[0].pos_y);   //affiche l'élément "nouvelle partie" non sélectionné

        ///La souris est sur la case reprendre une partie, donc on l'affiche avec un effet de sélection
        if(mouse_x>=elementA[3].pos_x && mouse_x<=elementA[3].taille_x && mouse_y>=elementA[3].pos_y && mouse_y<=elementA[3].taille_y)
        {
            draw_sprite(buffer, elementA[3].fichier, elementA[3].pos_x, elementA[3].pos_y);   //affiche l'élément "reprendre2" non sélectionné
            if(mouse_b&1)
                choix = 2;
        }
        else
        {
            //La souris n'est pas sur la case reprendre une partie, donc on l'affiche sans effet de sélection
            draw_sprite(buffer, elementA[2].fichier, elementA[2].pos_x, elementA[2].pos_y);   //affiche l'élément "reprendre" non sélectionné
        }

        ///La souris est sur la case aide, donc on l'affiche avec un effet de sélection
        if(mouse_x>=elementA[4].pos_x && mouse_x<=elementA[4].taille_x && mouse_y>=elementA[4].pos_y && mouse_y<=elementA[4].taille_y)
        {
            //affiche l'élément "aide & support" non sélectionné
            draw_sprite(buffer, elementA[5].fichier, elementA[5].pos_x, elementA[5].pos_y);

            if(mouse_b&1)
                choix = 3;
        }

        else
        {
            //La souris n'est pas sur la case aide donc on l'affiche normal
            draw_sprite(buffer, elementA[4].fichier, elementA[4].pos_x, elementA[4].pos_y);   //affiche l'élément "aide & support" non sélectionné
        }

        ///La souris est sur la case scores, donc on l'affiche avec un effet de sélection
        if(mouse_x>=elementA[6].pos_x && mouse_x<=elementA[6].taille_x && mouse_y>=elementA[6].pos_y && mouse_y<=elementA[6].taille_y)
        {
            draw_sprite(buffer, elementA[7].fichier, elementA[7].pos_x, elementA[7].pos_y);   //affiche l'élément "score" non sélectionné

            if(mouse_b&1)
                choix = 5;
        }

        else
        {
            //La souris n'est pas sur la case aide donc on l'affiche normal
            draw_sprite(buffer, elementA[6].fichier, elementA[6].pos_x, elementA[6].pos_y);   //affiche l'élément "score" non sélectionné
        }

        ///La souris est sur la case quitter, donc on l'affiche avec un effet de sélection
        if(mouse_x>=elementA[8].pos_x && mouse_x<=elementA[8].taille_x && mouse_y>=elementA[8].pos_y && mouse_y<=elementA[8].taille_y)
        {
            draw_sprite(buffer, elementA[9].fichier, elementA[9].pos_x, elementA[9].pos_y);   //affiche l'élément "quitter le jeu" non sélectionné

            if(mouse_b&1)
                choix = 4;
        }

        else
        {
            //La souris n'est pas sur la case aide donc on l'affiche normal
            draw_sprite(buffer, elementA[8].fichier, elementA[8].pos_x, elementA[8].pos_y);   //affiche l'élément "quitter" non sélectionné
        }

        if(key[KEY_ESC])
            choix = 4;

        draw_sprite_h_flip(buffer, souris, mouse_x-2, mouse_y-20);  //Afficher souris

        //Met à jour le buffer (permet d'afficher toutes les images en même temps)= pas de clignotements
        blit(buffer, screen, 0, 0, 0, 0, elementA[10].taille_x, elementA[10].taille_y);

    }
    while(!choix);

    destroy_bitmap(buffer);
    destroy_bitmap(souris);

    return choix;
}


void liberation_element(t_image *element, int j)
{
    int i;

    if(j==1)
    {
        for(i=0; i<NB_ELEMENTA; i++)
        {
            clear_bitmap(element[i].fichier);
        }
    }

    if(j==2)
    {
        for(i=0; i<NB_ELEMENTB; i++)
        {
            clear_bitmap(element[i].fichier);
        }
    }
}


