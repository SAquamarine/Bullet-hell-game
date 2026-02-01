#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#include "grille.h"
#include "pion.h"
#include "affichageMain.h"


void environnement_couleurs_black() { //fond noir
        init_pair(1, COLOR_MAGENTA, COLOR_BLACK); // magenta
        init_pair(2, COLOR_RED, COLOR_BLACK);     // rouge
        init_pair(3, COLOR_BLUE, COLOR_BLACK);   // bleu
        init_pair(4, COLOR_GREEN, COLOR_BLACK);  // vert
        init_pair(5, COLOR_YELLOW, COLOR_BLACK);  // jaune
        init_pair(6, COLOR_BLACK, COLOR_BLACK);   // noir
        init_pair(7, COLOR_WHITE, COLOR_BLACK);   // BACKGROUND
        bkgd(COLOR_PAIR(7));
}
void environnement_couleurs_white() { //fond blanc
        init_pair(1, COLOR_MAGENTA, COLOR_WHITE); 
        init_pair(2, COLOR_RED, COLOR_WHITE);     
        init_pair(3, COLOR_BLUE, COLOR_WHITE);   
        init_pair(4, COLOR_GREEN, COLOR_WHITE);  
        init_pair(5, COLOR_YELLOW, COLOR_WHITE);  
        init_pair(6, COLOR_WHITE, COLOR_WHITE);   
        init_pair(7, COLOR_BLACK, COLOR_WHITE);   
        bkgd(COLOR_PAIR(7));
}


int affichage_choix () {
        int choix;
        echo(); //on able echo pour que l'utilisateur voit ce qu'il tape
	printw("Bienvenue ! Que souhaitez-vous faire ? \n\n");
        printw("1/ Generer une grille d'une certaine taille \n");
        printw("2/ Generer une grille a partir d'un fichier\n");
        scanw(" %d", &choix);
        refresh();
        printw("Choix : %d\n", choix);
        refresh();
        noecho();
	return choix;
}

int affichage_option () {
        int option;
        echo(); 
        printw("\n\nChoisissez votre option  \n");
        printw("1/ Option 1 : le joueur ne se deplace que lorsqu'une touche est pressee \n");
        printw("2/ Option 2 : le joueur se deplace en continu\n");
        scanw(" %d", &option);
        refresh();
        printw("\nOption : %d\n", option);
        refresh();
        noecho();
        return option;
}


grilleEtPion * choixEgal1 () {
        int lign, col;
        echo();
	grilleEtPion * gEtp = allouer_gEtp();
        printw("Veuillez inserer les dimensions desirees (>=3) sous la forme lignes colonnes (puis taper sur la touche [ENTREE])\n");
	refresh();
        scanw(" %d %d", &lign, &col);
	while(lign<=3 || col<=3) { //tant que les dimensions ne sont pas bonnes
		printw("Mauvaises dimensions (%d x %d). Veuillez inserer des dimensions >=3 : \n", lign, col); refresh();
		scanw(" %d %d", &lign, &col);
	}
        gEtp->g = grille_initialiser(lign, col);
        grille_vider(gEtp->g);
        gEtp->p = allouer_pion(1, 1);
        placer_murs_contours(gEtp->g);
        placer_pion(gEtp->p, gEtp->g);
        gEtp->g->board[3][3] = PIEGE;
        noecho();
	return gEtp;
}


void optionEgal2 (grille * g, pion * p) {
	halfdelay(1);
        grille_dessiner (g);

        int direction = KEY_RIGHT;

        while(1) {
           int key = getch();
           if(key != ERR) {
            if (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT || key == 27){
                direction = key;
             }
            }
        deplacer_pion(p, g, direction);
        grille_dessiner(g);
    }
}


