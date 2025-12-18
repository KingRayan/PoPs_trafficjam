#include <stdio.h>
#include <time.h>
#include "console.h"
#include <stdlib.h>

#define MAX_LENGTH 80
#define MAX_HEIGHT 45

typedef struct {
	int x;
	int y;
}Point;

/* Fonction qui echange deux entiers en passage par pointeurs
 * @input : (int) x, (int) y
 * @return : NONE */
void swap(int *X, int *Y)
{
        int temp = *X; //save X
        *X = *Y; //cp Y in X
        *Y = temp; //cp saved X in Y
}



/* Fonction affichant un plateau vide
 * @input : (int) Longueur souhaitée
 * @return : affichage */
void generategrid(int gridl)
{
	/* Clear View */
	resetterminal();
	flushstdin();
	desactive_curseur();
	
	/* Espace libre (pour les informations) */
	int xspace = MAX_LENGTH - gridl; //espace (Left/Right) de notre fenetre de jeu
	int yspace = MAX_HEIGHT - gridl; //espace (Top/Bottom) de notre fenetre de jeu
	
	/* Fenetre de jeu */
	Point ptl = {31, 13}; //TopLeft
	Point ptr = {50, 13}; //TopRight
	Point pbl = {31, 31}; //BottomLeft
	Point pbr = {50, 31}; //BottomRight

	

	efface_ligne();
	allerxy(ptl.x, ptl.y);   	
	printf("+%.*s+", gridl, "==============================="); 

	for(int i = 1; i <= gridl; i++){
	
		if(i == ((pbl.y - ptl.y) / 2)){
			allerxy(ptl.x, (ptl.y + ptl.x)/2);
		       	printf("|%.*s>", gridl, "...............................");
		}
		else {
			allerxy(ptl.x, ptl.y + i);
			printf("|%.*s|", gridl, "...............................");
		}
	}
	allerxy(pbl.x, pbl.y);
	printf("+%.*s+", gridl, "==============================");
	//if (recupcar() == 'q') resetterminal(); // reset si l'utilisateur appuie sur q (énoncé)
}

/* Fonction affichant un véhicule rouge
 * @input : (Point) Rc{x,y} TopLeft de la voiture
 * 	    (int) hv horizontal(0) or Vertical(1) 
 * @return : affichage */

void generatemainCar(Point Rc, char direction, int color)
{
	int carlength = 6;
	int carheight = 3;
	
	if(direction != 'h') {
		swap(&carlength, &carheight);
	}
	
	couleurfond(color);
	//couleurpolice(31); // 30 Black, 31 Red, 32 Green, 33 Yellow, 34 Blue, 35 Magenta, 36 Cyan, 37 White
	// allerxy(Rc.x, Rc.y);
	for(int i = 0; i < carheight; i++){
			allerxy(Rc.x, Rc.y + i);
			printf("%.*s", carlength, "                     ");			
	}
	if (recupcar() == 'q') resetterminal(); // reset si l'utilisateur appuie sur q (énoncé)
	
}

int main(void)
{
	Point start = {32, 15};
	Point p2 = {40, 15};
        generategrid(18);
	generatemainCar(start, 'h', 31);
	generatemainCar(p2, 'v', 32);
	generatemainCar(zero, 'h', 33);
	
	generatemainCar(end, 'h', 35);
}
