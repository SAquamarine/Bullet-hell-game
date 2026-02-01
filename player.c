#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#include "grille.h"
#include "pion.h"
#include "affichageMain.h"
#include "player.h"
#include "game.h"


void manche_1 (grille * g, statistics * statsPlayer, statistics * statsBoss, pion * p) {
    int col[] = {7}; 
    attaqueBoss_horizontale (g,  col, 1, 120, statsPlayer, statsBoss, p);
    col[0] = 6;
    attaqueBoss_horizontale (g,  col, 1, 120, statsPlayer, statsBoss, p);     
    col[0] = 8;
    attaqueBoss_horizontale (g,  col, 1, 120, statsPlayer, statsBoss, p);

}

void manche_2 (grille * g, statistics * statsPlayer, statistics * statsBoss, pion * p) {
    int col2[] = {6, 8};
    int col3[] = {5, 7, 9};
    attaqueBoss_horizontale (g,  col2, 2, 120, statsPlayer, statsBoss, p);
    attaqueBoss_verticale (g, col2, 2, 120, statsPlayer, statsBoss, p);
    col2[0] = 7; col2[1] = 8;
    attaqueBoss_horizontale (g, col2, 2, 120, statsPlayer, statsBoss, p);
    attaqueBoss_verticale(g, col3, 3, 120, statsPlayer, statsBoss, p);
}


void tour_player (statistics * statsBoss, statistics * statsPlayer, inventaire * inv, grille * g) {
	int choix;
	int num;
	move(g->l+9, 5);
	printw( "\n\n\n     Votre tour \n 1/ Attaquer \t 2/ Inventaire \t 3/ Capituler \t  Choix  "); refresh();
	scanw(" %d", &choix); refresh();

	while (choix < 1 || choix > 3) { //si joueur ne prend pas une des options proposees
		printw("Veuillez choisir une des options ci-dessus\t  Choix : "); refresh();
		scanw(" %d", &choix); 
	}

	if(choix==2 && inv->cookie == 0 && inv->potion == 0) //si joueur veut ouvrir l'inventaire alors qu'il est vide 
	{
		printw("Vous ne pouvez pas ouvrir l'inventaire car ce dernier est vide \t  Choix  "); refresh();
		scanw(" %d", &choix);
	}

	if(choix==3) { //si le joueur decide d'abandonner
		printw("\nEtes vous sur de vouloir abandonner ? \t 1/ Oui\t 2/ Non     ");
		scanw(" %d", &choix);
		if(choix==2) { //si le joueur ne veut plus abandonner
			printw("Choix  ");
			scanw(" %d", &choix);
		}
		else choix = 3;
	}
	curs_set(0);

	switch (choix) {

	case 1 : //attaque

		printw("\n\nAttaques : \t 1/ Coup d'epee \t 2/ Boule de feu (coute un pv)\t"); refresh();
		scanw(" %d", &num);
		if(num==1) {
			printw("Vous assenez un coup d'epee au boss. Le boss perd 1 pv.\n"); refresh();
				statsBoss->pv -=1; napms(1000);
		} else {
			printw("Vous lancez une boule de feu au boss. Le boss perd 2 pv. Vous perdez 1 pv."); refresh();
			statsBoss->pv -= 2;
			statsPlayer->pv -= 1;
			napms(1000);
		}
		checks_dead_boss (statsBoss, g);
		afficher_pv_Boss (statsBoss, 5); refresh();
		checks_dead(statsPlayer, g);
		afficher_pv(statsPlayer, g->l+9);
		break;

	case 2 : //inventaire
		
		afficher_inventaire(inv, g->l+12);
		printw("\n\nQue voulez-vous utilisez ? 1/ Cookie 2/ Potion\t"); 
		if (inv->cookie == 0)
			printw("\nNote : il ne reste plus de cookie");
		else if (inv->potion == 0)
			printw("\nNote : il ne reste plus de potion");
		refresh();

		scanw(" %d", &num);
		if (num == 1) {
			if(inv->cookie == 0) {
				mvprintw(g->l+10, 2, "\nQue vous etes tetu ! Alors que vous fouillez votre sac \n");
				printw("a la recherche d'un cookie, le monstre en profite pour continuer le combat.");
				refresh(); napms(700);
				break;
			}
			inv->cookie -= 1;
			printw("Vous mangez le cookie. Vous regagnez 1 pv. Cookies restants : %d ", inv->cookie);
			statsPlayer->pv += 1;
			refresh(); getch();
		}
		else {
			if(inv->potion == 0) {
				mvprintw(g->l+10, 2,"\nQue vous etes tetu ! Alors que vous fouillez votre sac \n");
				printw("a la recherche d'une potion, le monstre en profite pour continuer le combat."); 
				refresh(); napms(700);
				break;
			}
			inv->potion -= 1;
			printw("Vous buvez la potion. Vous regagnez 2 pv. Potions restantes : %d ", inv->potion); 
			statsPlayer->pv += 2;
			refresh(), getch();
		}
		afficher_pv(statsPlayer, g->l+9);

		break;

	case 3 : //joueur veut capituler
		printw("Vous etes un trouillard et vous enfuyez du chateau. \n"); refresh(); napms(1000);
		attron(COLOR_PAIR(2));
		printw("GAME OVER "); refresh(); 
		attron(COLOR_PAIR(2));

		getch();
		endwin();
		exit(0);
		break;

	}
}
