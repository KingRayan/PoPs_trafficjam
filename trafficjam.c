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
	/* Attente de l'utilisateur */
	attendre(); // ne rien afficher sans choix de l'utilisateur par appui sur entree
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
	if (recupcar() == 'q') resetterminal(); // reset si l'utilisateur appuie sur q (énoncé)
}

/* Fonction affichant un véhicule rouge
 * @input : (Point) Rc{x,y} TopLeft de la voiture
 * 	    (int) hv horizontal(none) or Vertical(any) 
 * @return : affichage */

void generatemainCar(Point Rc, int hv)
{
	int carlength = 9;
	int carheight = 6;
	
	if(hv) {
		swap(&carlength, &carheight);
	}

	couleurpolice(12);
	allerxy(Rc.x, Rc.y);
	for(int i = 0; i < carheight; i++){
			allerxy(Rc.x, Rc.y + i);
			printf("%.*s", carlength, "**************");			
	}
	if (recupcar() == 'q') resetterminal(); // reset si l'utilisateur appuie sur q (énoncé)
	
}

int main(void)
{
	Point start = {32, 15};
	generategrid(18);
	generatemainCar(start, 0);
}
