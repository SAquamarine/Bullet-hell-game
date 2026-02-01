all: mainB

grille.o: grille.c grille.h
	gcc -c -Wall grille.c

pion.o: pion.c pion.h grille.h
	gcc -c -Wall pion.c

affichageMain.o: affichageMain.c pion.h grille.h
	gcc -c -Wall affichageMain.c

game.o: game.c affichageMain.h pion.h grille.h
	gcc -c -Wall game.c

player.o: player.c game.h affichageMain.h pion.h grille.h
	gcc -c -Wall player.c

mainA.o: mainA.c affichageMain.h pion.h grille.h
	gcc -c -Wall mainA.c

mainB.o: mainB.c affichageMain.h pion.h grille.h game.h
	gcc -c -Wall mainB.c

mainB: mainB.o grille.o pion.o affichageMain.o game.o player.o
	gcc -o mainB mainB.o grille.o pion.o affichageMain.o game.o player.o -lncursesw

clean:
	rm -f *.o mainB
