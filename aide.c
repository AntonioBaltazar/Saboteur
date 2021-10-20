#include "header.h"

/// AFFICHAGE PAGES REGLEMENT
void aide()
{
    int i=0;
    int cmpt=0;

    ///Déclaration d'un tableau d'image et son allocation dynamique d'espace mémoire
    t_image *image;
    image = (t_image*)malloc(sizeof(t_image)*10);

    /// CREATION BUFFER
    BITMAP *buffer;
    buffer = create_bitmap(800,600);

    ///Blindage sur buffer et image
    if(buffer == NULL || image == NULL)
        allegro_message("Erreur : la creation du buffer pour la fonction aide a echoue");
    else
    {
        ///Préparation des noms des fichiers pour le telechargement des images
        sprintf(image[0].nomfichier,"aide/P1.bmp");
        sprintf(image[1].nomfichier,"aide/P1A.bmp");
        sprintf(image[2].nomfichier,"aide/P1B.bmp");
        sprintf(image[3].nomfichier,"aide/P2.bmp");
        sprintf(image[4].nomfichier,"aide/P2A.bmp");
        sprintf(image[5].nomfichier,"aide/P2B.bmp");
        sprintf(image[6].nomfichier,"aide/P3.bmp");
        sprintf(image[7].nomfichier,"aide/P3A.bmp");
        sprintf(image[8].nomfichier,"aide/P3B.bmp");
        sprintf(image[9].nomfichier,"aide/fond.bmp");

        image[9].fichier = load_bitmap(image[9].nomfichier,NULL);


        ///Telechargement des images
        for(i=0; i<9; i++)
        {
            image[i].fichier = load_bitmap(image[i].nomfichier,NULL);
            image[i].pos_x = 150;
            image[i].pos_y = 10;
            image[i].taille_x = 570;
            image[i].taille_y = 570;

            if(image[i].fichier == NULL)
                allegro_message("Erreur de telechargement du reglement");
        }

        allegro_message("Vous pouvez utiliser les fleches directionnelles ou les fleches en bas de page pour tourner les pages !");

        ///On affiche la souris à l'écran
        show_mouse(screen);

        do
        {
            clear_bitmap(buffer);
            ///Affichage de l'image de fond
            stretch_sprite(buffer,image[9].fichier,0,0,800,600);

            if(key[KEY_RIGHT])
            {
                cmpt+=3;
                rest(270);
            }

            if(key[KEY_LEFT])
            {
                if(cmpt==0)
                {
                    cmpt=9;
                    rest(270);
                }
                else
                {
                    cmpt-=3;
                    rest(270);
                }
            }

            ///Page suivante
            if((mouse_x>635)&&(mouse_x<665)&&(mouse_y>560)&&(mouse_y<578))
            {
                stretch_sprite(buffer,image[cmpt+1].fichier, image[cmpt+1].pos_x, image[cmpt+1].pos_y, image[cmpt+1].taille_x, image[cmpt+1].taille_y);

                if(mouse_b&1)
                {
                    cmpt+=3;
                    rest(270);
                }
            }

            ///Page précédente
            else if((mouse_x>600)&&(mouse_x<628)&&(mouse_y>560)&&(mouse_y<578))
            {
                stretch_sprite(buffer,image[cmpt+2].fichier,image[cmpt+2].pos_x, image[cmpt+2].pos_y, image[cmpt+2].taille_x, image[cmpt+2].taille_y);

                if(mouse_b&1)
                {
                    cmpt-=3;
                    rest(270);
                }
            }
            ///Page normale
            else
                stretch_sprite(buffer,image[cmpt].fichier,image[cmpt].pos_x, image[cmpt].pos_y, image[cmpt].taille_x, image[cmpt].taille_y);

            blit(buffer,screen,0,0,0,0,800,600);

        }
        while(cmpt<9 && cmpt>-1);
    }

    ///Libération mémoire
    free(image);
    destroy_bitmap(buffer);
}
