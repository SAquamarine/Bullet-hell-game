#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#include "grille.h"
#include "pion.h"
#include "affichageMain.h"
#include "game.h"



statistics * allouer_stat (int pv) {
	statistics * stat = malloc(sizeof(statistics));
	stat->pv = pv;
	return stat;
}

void desallouer_stats (statistics * stat) {
	free(stat);
}

void afficher_pv (statistics * stat, int ligne) {
	move( ligne - 2, 5);
	printw ("PV JOUEUR      ");
	for (int i=0; i < stat->pv; i++) {
		attron(COLOR_PAIR(2)); //s'affiche en rouge
		printw( "♥ ");
	}
	attroff(COLOR_PAIR(2));
}

void afficher_pv_Boss (statistics * stat, int ligne) {
	move( ligne, 5);
	printw ("PV BOSS      ");
	for (int i=0; i < stat->pv; i++) {
		attron(COLOR_PAIR(4)); //s'affiche en vert
		printw( "♥ ");
	}
	attroff(COLOR_PAIR(4));
}

void checks_dead (statistics * stat, grille * g) {
	if (stat -> pv == 0) {
		nodelay(stdscr, FALSE);
		afficher_pv(stat, g->l+9); refresh();
		printw("\n\nVous êtes mort pendant le combat...\n"); refresh();
		napms(1000);
		attron(COLOR_PAIR(2));
		printw("GAME OVER\n"); refresh();
		napms(1000);
		free(stat); free(g);

		getch();
		attroff(COLOR_PAIR(2));
		endwin();
		exit(0);
	}
}

void checks_dead_boss(statistics * stat, grille * g) {
	if (stat->pv == 0) {
		afficher_pv_Boss(stat, g->l+9); refresh();
		// vide les lignes precedentes pour l'affichage du message de victoire
		for (int i = g->l + 9; i < g->l + 20; i++) {
			move(i, 0);
			clrtoeol();
		}
		refresh();
		move(g->l + 10, 2);
		printw("\nVous avez vaincu le boss.\n");
		refresh();
		napms(1000);
		attron(COLOR_PAIR(4));
		printw("Bravo !!!\n"); refresh();

		free(stat); free(g);
		getch();
		attroff(COLOR_PAIR(4));
		napms(500);
		endwin();
		exit(0);
	}
}

inventaire * allouer_inventaire() {
        inventaire * inv = malloc(sizeof(inventaire));
        inv->cookie = 2;
        inv->potion = 2;
        return inv;
}

void desallouer_inventaire(inventaire * inv) {
	free(inv);
}


void afficher_inventaire(inventaire * inv, int ligne) {
	move(ligne, 5);
	printw("\n--- Inventaire ---\n\n");
	printw("Cookie : x%d\t         Description : recharge 1 pv\n", inv->cookie);
	printw("Potion de vie : x%d \t Description : recharge 2 pv", inv->potion);
}


/*fonctions d'attaque du boss*/


void rempli_aleatoirement_tab (int * tab, int n) { //permet de generer des tableaux d'attaques aleatoires 
	for(int i = 0; i < n; i++) { //genere les numeros des colonnes ou lignes ou il va y avoir des attaques
		tab[i] = rand() % 11 + 2; //avec le 11+2, on evite de remplacer les murs
	}
}

void collision_attaque_hori_player(grille * g, statistics * statsPlayer, int ligne, int i, pion * p) { //verifie si une attaque touche le joueur ou pas

    if (g->board[ligne][i] == PION) {
        statsPlayer->pv -= 1;
        checks_dead(statsPlayer, g);
        g->board[p->x][p->y] = RIEN;

        p->x = (ligne > 1) ? (ligne - 1) : (ligne + 1); //s'il y a collision, on deplace le joueur sur la ligne du dessus ou du dessous en fonction de la presence de murs
        p->y = i;  
        placer_pion(p, g); 
    }
}

void collision_attaque_verti_player (grille * g, statistics * statsPlayer, int col, int i, pion * p) {

    if (g->board[i][col] == PION) {
        statsPlayer->pv -= 1;
        checks_dead(statsPlayer, g);
        g->board[p->x][p->y] = RIEN;

		p->y = (col < g->c - 1) ? (col + 1) : (col - 1); //s'il y a collision, on deplace le joueur sur la colonne de droite ou de gauche en fonction de la presence de murs
        p->x = i;  
        placer_pion(p, g); 
    }
}

void attaqueBoss_horizontale(grille * g, int lignes[], int nb_lignes, int vitesse, statistics * statsPlayer, statistics * statsBoss, pion * p) {
    nodelay(stdscr, TRUE);    

    for (int i = 1; i < g->c - 1; i++) { 
        int key = getch(); 
        if (touche_valide(key)) { //si le joueur appuye sur echap
            if (key == 27) {
                mvprintw(30, 0, "Vous avez appuyé sur la touche ECHAP. \nA bientot !");
                refresh();
				free(g); free(statsPlayer); free(statsBoss); free(p);
                napms(1000);
                endwin();
                exit(0);
            }
            g->board[p->x][p->y] = RIEN;
            deplacer_pion(p, g, key); 
        }

        for (int j = 0; j < nb_lignes; j++) {
            collision_attaque_hori_player(g, statsPlayer, lignes[j], i, p);
            g->board[lignes[j]][i] = ATTAQUE;
        }

        placer_pion(p, g);
        grille_dessiner(g); 
        afficher_pv(statsPlayer, g->l + 9);
        afficher_pv_Boss(statsBoss, 5);
        refresh(); 
        napms(vitesse);

        for (int j = 0; j < nb_lignes; j++) 
            g->board[lignes[j]][i] = RIEN; //on vide l'emplacement de l'attaque
        
    }
    nodelay(stdscr, FALSE);
}


void attaqueBoss_verticale (grille * g, int col[], int nb_col, int vitesse, statistics * statsPlayer, statistics * statsBoss, pion * p) {
	nodelay(stdscr, TRUE);	

	for ( int i=1; i < g->l - 1; i++) { 

		int key = getch(); 
		if(touche_valide(key)) {
            if (key == 27) { // si touche echap
                mvprintw(30, 0, "Vous avez appuyé sur la touche ECHAP. \nA bientot !");
                refresh();
				free(g); free(statsPlayer); free(statsBoss); free(p);

                napms(1000);
                endwin();
                exit(0);
            }
            g->board[p->x][p->y] = RIEN;
			deplacer_pion(p, g, key); 
		}
		//on verifie s'il y a collision a chacune des lignes ou l'attaque passe
		for(int j=0; j<nb_col;j++) {
			collision_attaque_verti_player(g, statsPlayer, col[j], i, p);
			g->board[i][col[j]] = ATTAQUE;
		}

		placer_pion(p, g);
		grille_dessiner(g); 
		afficher_pv(statsPlayer, g->l+9);
		afficher_pv_Boss(statsBoss, 5);
		refresh(); 
		napms(vitesse);

		for(int j=0; j<nb_col;j++) 
			g->board[i][col[j]] = RIEN;
			
	}
	nodelay(stdscr, FALSE);
}


void attaqueBoss_horizontale_et_verticale(grille *g, int lignes[], int nb_lignes, int cols[], int nb_cols ,int vitesse, statistics * statsPlayer, statistics * statsBoss, pion * p) 
{
    nodelay(stdscr, TRUE);
	int nb_etapes_max;
    if (g->l > g->c) nb_etapes_max = g->l ;
	else nb_etapes_max = g->c;

    for (int i = 1; i < nb_etapes_max - 1; i++) {
        int key = getch();
        if (touche_valide(key)) {
            if (key == 27) {
                mvprintw(30, 0, "Vous avez appuyé sur la touche ECHAP. \nA bientot !");
                refresh();
				free(g); free(statsPlayer); free(statsBoss); free(p);

                napms(1000);
                endwin();
                exit(0);
			}
            g->board[p->x][p->y] = RIEN;
            deplacer_pion(p, g, key);
        }

        // attaques horizontales
        if (i < g->c - 1) {
            for (int j = 0; j < nb_lignes; j++) {
                collision_attaque_hori_player(g, statsPlayer, lignes[j], i, p);
                g->board[lignes[j]][i] = ATTAQUE;
            }
        }

        // attaques verticales
        if (i < g->l - 1) {
            for (int j = 0; j < nb_cols; j++) {
                collision_attaque_verti_player(g, statsPlayer, cols[j], i, p);
                g->board[i][cols[j]] = ATTAQUE;
            }
        }

        placer_pion(p, g);
        grille_dessiner(g);
        afficher_pv(statsPlayer, g->l + 9);
        afficher_pv_Boss(statsBoss, 5);
        refresh();
        napms(vitesse);

		// on vide l'emplacement precedent de l'attaque
		if (i < g->c - 1) {        // horizontalement
            for (int j = 0; j < nb_lignes; j++) {
                g->board[lignes[j]][i] = RIEN;
            }
        }
    
        if (i < g->l - 1) { //verticalement
            for (int j = 0; j < nb_cols; j++) {
                g->board[i][cols[j]] = RIEN;
			}
        }
	}

    nodelay(stdscr, FALSE);
}


