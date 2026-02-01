#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "pion.h"
#include "grille.h"


pion * allouer_pion (int x, int y) {
	pion * p = malloc (sizeof(pion));
	p->x = x;
	p->y = y;
	return p;
}

void desallouer_pion (pion * p) {
	free(p);
}

void placer_pion (pion * p, grille * g) {
	g->board[p->x][p->y] = PION;
}

int touche_valide(int key) { //verifie si la touche pressee est valide ou pas
    if (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT || key == 27)
        return TRUE;
    else return FALSE;

}

void deplacer_pion (pion * p, grille * g, int key) {
   pion * pO = allouer_pion(p->x, p->y); //stock placement precedent

   switch (key) {
        case KEY_RIGHT :
                p->y += 1; break;
        case KEY_LEFT :
                p->y -= 1; break;
        case KEY_DOWN :
                p->x += 1; break;
        case KEY_UP :
                p->x -= 1; break;
	case 27 :
                mvprintw(30, 0, "Vous avez appuyé sur la touche ECHAP. \nA bientot !");
		refresh();
                free(p); free(g);
	        napms(1000); //attends 500 ms avant de quitter prog
                getch();
	    	endwin();
		exit(0);
           default :
                   return;
   }
 //on cheque si l'emplacement est valide et si la case n'est pas un mur (derniere condition)
   if(p->x >= 0 && p->x < g->l && p->y >= 0 && p->y < g->c && g->board[p->x][p->y] != MUR) {
	if(g->board[p->x][p->y] == PIEGE) { //si on tombe sur un piege, la partie s'arrete
		placer_pion(p, g);
		g->board[pO->x][pO->y] = RIEN;
		grille_dessiner(g);
                mvprintw(30, 0, "Vous êtes tombé dans un piège... \nSoyez plus prudent la prochaine fois. \n");
                refresh();
                free(g); free(p);
                getch();
                napms(1000); //attends 1000 ms avant de quitter prog
                endwin();
                exit(0);
	}

      g->board[pO->x][pO->y] = RIEN;   //on vide l'ancien emplacement
      placer_pion(p, g);
   }
   else {
           p->x = pO->x;
           p->y = pO->y;
   }
   free(pO);
}

//allocation d'un struct regroupant une grille et un pion
grilleEtPion * allouer_gEtp () {
        grilleEtPion * gEtp = malloc(sizeof (grilleEtPion));
        gEtp -> g = grille_initialiser (0, 0);
        gEtp -> p = allouer_pion (0, 0);
        return gEtp;

}
//lecture de la grille a partir d'un fichier
grilleEtPion * grille_charger_fichier (const char * fich) {
  FILE *fic = fopen(fich, "r");
  char buffer[255];
  int l, c;

  if (!fic){ //si le fichier n'existe pas
    printw("Fichier non trouvé\n"); refresh();
    return NULL;
  }

//Etape 1 : on preleve les coordonnees
//lecture du fichier
if (fgets(buffer, sizeof(buffer), fic) == NULL) {
        printw("Fichier inexistant\n");
        refresh();
        fclose(fic);
        return NULL;
}

    if(sscanf(buffer, "%d %d", &l, &c) != 2) {
                printw("erreur de dimensions");
                refresh(); fclose(fic);
                return NULL;
        }


    grilleEtPion * gEtp = allouer_gEtp();  //on alloue un struct couplant grille et pion pour pouvoir retourner l'emplacement du pion
    gEtp -> g = grille_initialiser (l, c);
    if (!gEtp -> g) {
        fclose(fic);
        return NULL;
        }

//Etape 2 : on remplit la grille

   int i, j=0;

        for(i=0; i<l && fgets(buffer, sizeof(buffer), fic) !=NULL; i++) {
                for(j=0; j<c && buffer[j] != '\0' && buffer[j] != '\n'; j++) {
                        char ch = buffer[j];
                        switch (ch) {
                                case '#' :
                                case 'M' : //si le cara est un mur symbolise par # ou M
                                        gEtp -> g -> board[i][j] = MUR;
                                        break;
                                case 'P' : //si c'est un pion
                                        gEtp -> g -> board[i][j] = PION;
                                        gEtp -> p = allouer_pion(i, j); //on stocke l'emplacement du pion
                                        break;
                                case 'B' :
                                        gEtp -> g -> board[i][j] = BUTIN;
                                        break;
                                case 'X' :
                                        gEtp -> g -> board[i][j] = PIEGE;
                                        break;
                                case ' ' : //si rien
                                        gEtp -> g -> board[i][j] = RIEN;
                                        break;
                                default :
                                        gEtp -> g -> board[i][j] = RIEN ; //si on ne sait pas ce qui est sur la case
                                        break;
                        }
                }
        }

fclose(fic);
return gEtp;
}
