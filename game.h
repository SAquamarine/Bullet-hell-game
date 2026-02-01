#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "grille.h"
#include "pion.h"
#include "affichageMain.h"

#define TRUE 1
#define FALSE 0

typedef struct {
	int pv; //points de vie
} statistics;

typedef struct {
	int cookie;
	int potion;
} inventaire;

//allocation et desallocation des stats
statistics * allouer_stat (int pv);
void desallouer_stats (statistics * stat);

//affiche les pv (fonctions differentes pour le joueur et le boss car les msg sont differents)
void afficher_pv (statistics * stat, int ligne);
void afficher_pv_Boss (statistics * stat, int ligne);

//verifie si le joueur ou le boss sont morts (fonctions differentes pour le joueur et le boss car les msg sont differents)
void checks_dead (statistics * stat, grille * g);
void checks_dead_boss (statistics * stat, grille * g);


inventaire * allouer_inventaire();
void desallouer_inventaire(inventaire * inv);
void afficher_inventaire(inventaire * inv, int ligne);

//attaques

void rempli_aleatoirement_tab(int * tab, int n);
void collision_attaque_hori_player(grille * g, statistics * statsPlayer, int ligne, int i, pion * p); //verifie si le joueur est touche par une attaque horizontale
void collision_attaque_verti_player(grille * g, statistics * statsPlayer, int col, int i, pion * p); //verifie si le joueur est touche par une attaque verticale
void attaqueBoss_verticale (grille * g, int col[], int nb_col, int vitesse, statistics * statsPlayer, statistics * statsBoss, pion * p); //attaque verticale du boss
void attaqueBoss_horizontale(grille * g, int lignes[], int nb_lignes, int vitesse, statistics * statsPlayer, statistics * statsBoss, pion * p); //attaque horizontale du boss
void attaqueBoss_horizontale_et_verticale(grille *g, int lignes[], int nb_lignes, int cols[], int nb_cols, int vitesse, statistics *statsPlayer, statistics *statsBoss, pion *p); 
//attaque horizontale et verticale du boss



#endif

