#ifndef _GAME_H_
#define _GAME_H_

#include "grille.h"
#include "pion.h"
#include "affichageMain.h"
#include "game.h"



/*note : les fonctions manche_1 et manche_2 sont pratiquement les memes, mais elles permettent de bien debuter la partie et de laisser 
le temps au joueur de comprendre le jeu*/
void manche_1 (grille * g, statistics * statsPlayer, statistics * statsBoss, pion * p);
void manche_2 (grille * g, statistics * statsPlayer, statistics * statsBoss, pion * p);

void tour_player (statistics * statsBoss, statistics * statsPlayer, inventaire * inv, grille * g);



#endif
