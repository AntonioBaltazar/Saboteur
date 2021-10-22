#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
#include <math.h>
#include <algif.h>

#define NB_ELEMENTA 11
#define NB_ELEMENTB 23



typedef struct image
{
    BITMAP *fichier;
    char nomfichier[30];
    int pos_x;
    int pos_y;
    int taille_x;
    int taille_y;
    int mouvementx;
    int mouvementy;

} t_image;

typedef struct carte    ///Structure d'une carte
{

    char valeur[30];
    struct carte * suivant;

} t_carte;

typedef struct Pile     ///La pile de jeu
{

    t_carte * premier;

} t_Pile;

typedef struct joueur    ///Structure pour chaque joueur
{
    char role[10];          //saboteur, chercheur
    char pseudo[16];
    int outils[3];          //charette, pioche, lanterne: Cassés ou pas
    int score;
    char cartes[7][10];       //6 cartes dans la main + 1 quand on pioche
    int logo;               //Image du nain
} t_joueur;

///Sous-programme concernant le début de la partie
void ChoisirIconesNoms();
void initJoueur();
void prendreLogo();
void ChoixPseudo();
void initNains();
void initSortie();

///Sous-programme concernant le menu
void menu();
void initialisation_allegro();
int menu_principal(t_image *element);
void initialisation_image_menu_principal(t_image *elementA);
void liberation_element(t_image *element, int j);
void initialisation_image_nouvelle_partie(t_image* element);

void gif();
void initialisation_gif(t_image *gif);


void aide();
void nouvelle_partie();
void choix_nb_joueurs(t_image *element,BITMAP *buffer, int *choix, int *validation, int *action);
void initialisation_choix_nb_joueur(t_image* element);

///Initialisater et utiliser la pioche
void initialiserJeu();
void melangerJeu(t_carte *tab,t_carte *tabmelangee);
void creationPioche(t_Pile * pile,t_carte *tabmelangee);
char* piocher(t_Pile * pile);

///Sous-programme concernant le jeu
void MainPlateau();//Programme de la boucle du jeu
void Plateau();//Un tour de joueur

void afficherCartes();
void afficherAdversaires();
void AfficherJoueur();
void afficherPlateau();

///Actions faites par les joueurs
void dragAndDrop();
void ActionsJoueur();
void PlacerCarte();
void eboulement();
void detruireGalerie();
void Reparer();
void Detruire();

void initCartes();
void outilsAdverses();
int confirmer();
void VoirJoueurs();
void verifierPlateau();
void connectionCarte();
void afficherMessage();

///Sous-programme concernant l'ia
void TourOrdi();//Programme principal: tour de l'ia

///Actions
void determinerActionsPossibles();
void detruireCarte();
void casser();
void reparer();
void creuser();
void defausserCarte();
int verifierPlateauOrdi();
void Resume();

///Sauvegardes
void ReprendreJeu();
void Sauvegarder();
void Reprendre();
void SauvegarderPile();
void ReprendrePile();
void ajouterCarte();
void sauvegarde_score();

///Fin de partie
void FinDePartie();
void GagnerPepites();
void parcourir();
void meilleur_joueur();

///Ecrans de victoire
void victoire_chercheur();
void game_over();
void victoire_saboteur();
void mouvement_ecran_saboteur();
void telechargement_victoire_saboteur();

#endif // HEADER_H_INCLUDED

