#ifndef _GRILLE_H_
#define _GRILLE_H_

enum obj { RIEN, MUR, PION, PIEGE, BUTIN, ATTAQUE};
typedef struct {
	int l, c;
	enum obj ** board; //tableau qui contient tous les emplacements des elements de la grille
} grille;


grille * grille_initialiser (int l, int c);
void grille_desallouer (grille * g);
void grille_vider (grille * g);
void grille_dessiner (grille * g);
void placer_murs_contours(grille * g);

#endif
