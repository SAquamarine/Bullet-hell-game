#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <time.h>

#include "grille.h"
#include "pion.h"
#include "affichageMain.h"
#include "player.h"
#include "game.h"


int main() {

        setlocale(LC_ALL, "");

        initscr();
        raw();
        keypad(stdscr, TRUE);
        echo();
        curs_set(0);
        start_color();
        environnement_couleurs_white(); //pour avoir des couleurs dans le jeu avec un fond blanc
        int scrLign, scrCol;
        getmaxyx(stdscr, scrLign, scrCol); // recupere la taille de l'ecran
        (void)scrLign; //on met cette ligne pour pas qu'un warning "set but not used" apparaisse  
        srand(time(NULL));

        int i = 0;
        int choix;
        grille * g = grille_initialiser(15 , 15);
        pion * p = allouer_pion(7, 7);
        placer_murs_contours(g);

        statistics * statsPlayer = allouer_stat(6); //joueur a 6 pv
        statistics * statsBoss = allouer_stat(5); //boss a 5 pv
        inventaire * inv = allouer_inventaire();
        
        mvprintw( 2, (scrCol/2) - 5, " --- MISSION --- \n\n\n");


        printw(" Vous etes un chevalier et allez vaincre le monstre qui terrifie le village. \n Debarassez vous de lui !\n"); refresh();
        printw("\n\nVous avez un inventaire. Voulez-vous le consulter ?\t 1/ Oui\t 2/ Non\t ");
        scanw(" %d", &choix);
        if(choix == 1) {
                afficher_inventaire(inv, 7); 
        }
        printw("\n\nAppuyez sur une touche pour debuter le combat.\n\n"); refresh(); getch();

        grille_dessiner(g); 
        
        move(3, 5);
        printw("Le monstre vous attaque ! Esquivez ses attaques\n"); 
        afficher_pv(statsPlayer, g->l+9);
        afficher_pv_Boss(statsBoss, 5);
        refresh(); getch();

        int a = 3;

        while (1) { /*on ne met pas de condition sur les pv du joueur ou du boss car 
                les fonctions s'occupent deja de ces cas */

                if (i==0) {
                        manche_1 (g, statsPlayer, statsBoss, p);
                }                        
                
                else if(i==1) {
                        manche_2 (g, statsPlayer, statsBoss, p);
                        i=2; //on incremente i pour pouvoir avoir un bon nombre d'attaques dans le prochain if
                }

                else if (i<6) {
                        for(int j=0; j<i; j++) { //on attaque i fois
                                int *col = malloc(a * sizeof(int));
                                rempli_aleatoirement_tab(col, a);
                                if(rand()%6 == 0 || rand()%6 == 1) {
                                        attaqueBoss_verticale (g, col, a, 115, statsPlayer, statsBoss, p);
                                }
                                else if (rand()%6 == 2 || rand()%6 == 3) {
                                        attaqueBoss_horizontale (g, col, a, 115, statsPlayer, statsBoss, p);
                                }    
                                else  { //on a 1/6 de chance de recrevoir une double attaque
                                        attaqueBoss_horizontale_et_verticale (g,  col, a, col, a, 115, statsPlayer, statsBoss, p);
                                }
                                free(col);
                        }
                }

                else {
                        for(int j=0; j<i; j++) {
                                a = 6;
                                int *col = malloc(a * sizeof(int));
                                rempli_aleatoirement_tab(col, a);
                                if(rand()%4 == 0) {
                                        attaqueBoss_verticale (g, col, a, 110, statsPlayer, statsBoss, p);
                                }
                                else if (rand()%4 == 1) {
                                        attaqueBoss_horizontale (g, col, a, 110, statsPlayer, statsBoss, p);
                                }    
                                else  { //ici, 1 chance sur deux de recevoir une double attaque
                                        attaqueBoss_horizontale_et_verticale (g,  col, a, col, a, 110, statsPlayer, statsBoss, p);
                                }
                                free(col);
                        }
                }

                // tour joueur
                tour_player(statsBoss, statsPlayer, inv, g);
                
                i++;
                a++;
        }


	getch();
        endwin();
	return 0;
}
