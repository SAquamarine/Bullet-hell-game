#ifndef _PION_H_
#define _PION_H_

#include "grille.h"

typedef struct {
	int x, y;
} pion;

typedef struct {
        grille * g;
        pion * p;
} grilleEtPion; //struct pour les fonctions de lecture d'un fichier

enum event {HAUT, BAS, DROITE, GAUCHE, ECHAP};


pion * allouer_pion (int x, int y);
void desallouer_pion (pion * p);
void placer_pion (pion * p, grille * g);
void deplacer_pion (pion * p, grille * g, int key);

int touche_valide(int key);

grilleEtPion * allouer_gEtp ();
grilleEtPion * grille_charger_fichier (const char * fich);

#endif
