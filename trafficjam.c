#include <stdio.h>
#include <time.h>
#include "console.h"
#include <stdlib.h>


/* INFORMATIONS SUR LA GRID */
#define MAX_LENGTH 80
#define MAX_HEIGHT 45
#define GRID_SIZE 18
#define CELL 3
#define MAX_VEHICULES 10
#define GRID_ORIGIN_X 32
#define GRID_ORIGIN_Y 15

/* COULEURS DE TEXTE (VEHICULES) */
#define COLOR_BLACK 30
#define COLOR_RED 31
#define COLOR_GREEN 32
#define COLOR_YELLOW 33
#define COLOR_BLUE 34
#define COLOR_PURPLE 35
#define COLOR_CYAN 36
#define COLOR_WHITE 37

typedef struct {
	int id; // référencer les véhicules de la grille
	int x, y; // top-left point vehicule
	char dir; // 'h'(horizontal) or 'v' (vertical)
	int length; // 2(car) or 3(truck)
	int color; //differencier la main car coté UX 
}Vehicule;


typedef struct{
	Vehicule cars[MAX_VEHICULES]; //vehicules de la grille (array)
	int count;
	int selected;
}Game;

int main(void)
{
	printf("Memory occupation :\n");
	printf("sizeof(Vehicle)  = %lu bytes\n", sizeof(Vehicule));
    	printf("sizeof(GameState)= %lu bytes\n", sizeof(Game));
	return 0;	
}
