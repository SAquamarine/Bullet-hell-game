#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>

#include "grille.h"

//initialisation d'une grille l x c 
grille * grille_initialiser (int l, int c) {
	grille * g = malloc(sizeof(grille));
	if (g == NULL) {
		printw("echec d'allocation de l'espace memoire\n");
		exit(2);
	}
	g->board = malloc(sizeof(enum obj *) * l);
	if (g->board == NULL) {
		printw("echec de l'allocation de l'espace memoire\n");
		exit(2);
	}
	for(int i=0; i<l; i++) {
		g->board[i] = malloc ( sizeof(enum obj) * c);
		if (g->board[i] == NULL) {
			printw("echec d'allocation de l'espace memoire\n");
			exit(2);
		}
	}
	g->l = l;
	g->c = c;
	return g;
}


void grille_desallouer (grille * g) {
	for(int i=0; i<g->l; i++) {
		free(g->board[i]);
	}
	free(g->board);
	free(g);
}

//on remplit chaque case par RIEN
void grille_vider (grille * g) {
	int i, j;
	for(i=0; i<g->l; i++) {
		for(j=0; j<g->c; j++) {
			g->board[i][j] = RIEN ;
		}
	}
}

//on place des murs sur les contours de la grille
void placer_murs_contours(grille * g) {
	int i, j;
	for(i = 0; i < g->l; i++) {
		for(j = 0; j < g->c; j++) {
			g->board[0][j] = MUR;
			g->board[g->l - 1][j] = MUR;
		}
		g->board[i][0] = MUR;
		g->board[i][g->c -1] = MUR;
	}

}


//dessine la grille au milieu de l'ecran
void grille_dessiner(grille *g) {
    erase();

    int scrLign, scrCol;
    getmaxyx(stdscr, scrLign, scrCol); // recupere la taille de l'ecran

    int longu_grille = g->c * 2;  
    int larg_grille = g->l;

    int y = (scrLign - larg_grille) / 2;
    int x = (scrCol - longu_grille) / 2;

    char sym;
    for (int i = 0; i < g->l; ++i) {
        for (int j = 0; j < g->c; ++j) {
            int color_pair = 0; //prend un set de couleur definit dans une des fonctions environnement_couleurs

            switch (g->board[i][j]) {
                case RIEN:
                    sym = ' ';
                    color_pair = 5;
                    break;
                case MUR:
                    sym = '#';
                    color_pair = 4;
                    break;
                case PION:
                    sym = 'P';
                    color_pair = 2;
                    break;
                case PIEGE:
                    sym = 'X';
                    color_pair = 1;
                    break;
                case BUTIN:
                    sym = 'B';
                    color_pair = 5;
                    break;
                case ATTAQUE:
                    sym = 'A';
                    color_pair = 2;
                    break;
                default:
                    sym = '?';
                    color_pair = 3;
                    break;
            }

            attron(COLOR_PAIR(color_pair));
            if (sym == 'P')
                mvprintw(y + i - 3, x + j * 2, "♥");
            else if (sym == 'A')
                mvprintw(y+i - 3,  x + j * 2, "☼");
            else
                mvprintw(y + i - 3, x + j * 2, "%c", sym);

            attroff(COLOR_PAIR(color_pair)); //retire la couleur
        }
    }
}
