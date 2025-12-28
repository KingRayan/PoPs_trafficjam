#include <stdio.h>
#include <time.h>
#include "console.h"
#include <stdlib.h>


/* INFORMATIONS SUR LA GRID */
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
	int id; 		// référencer les véhicules de la grille
	int x, y; 		// top-left point vehicule
	char dir; 		// 'h'(horizontal) or 'v' (vertical)
	int length; 		// 2(car) or 3(truck)
	int color; 		// differencier les voitures coté UX 
}Vehicule;


typedef struct{
	Vehicule Cars[MAX_VEHICULES]; 		// vehicules de la grille 
	int count; 		     		// nb vehicules actifs
	int selected;				// id vehicule selected
}Game;

/* =================== LOGIQUE ====================== */

void build_grid(int grid[GRID_SIZE][GRID_SIZE], Game *s)
{
	int x, y; 		// iteration sur la grille (x, y)
	int i;	  		// iteration sur les vehicules
	int dx, dy;		// iteration sur un vehicule(dx, dy)

	for(y = 0; y < GRID_SIZE; y++)
		for(x = 0; x < GRID_SIZE; x++) 
			grid[y][x] = -1; // -1 == empty 

	for(i = 0; i < s->count; i++)
	{
		Vehicule *v = &s->Cars[i]; // pointe sur le vehicule actuel

		/* mise a echelle par le facteur multiplicateur sur la grille */
		/* i.e grid 6x6 => 18x18, donc OBLIGATOIREMENT pour les vehicules modifier longueur OU hauteur dépendamment de la direction */	
		int len = (v->dir == 'h') ? v->length * CELL : CELL;	// horizontal <=> length*CELL
		int hei = (v->dir == 'v') ? v->length * CELL : CELL;	// vertical   <=> height*CELL
		
		for(dy = 0; dy < hei; dy++)
			for(dx = 0; dx < len; dx++)
				grid[v->y + dy][v->x + dx] = v->id; // associer les grilles occupées aux véhicules distincts
	
	}

}

/* ================== RENDERING ======================== */

void draw_grid(void)
{
	int i;
	allerxy(GRID_ORIGIN_X - 1, GRID_ORIGIN_Y - 1);
	printf("+==================+");

 	for (i = 0; i < GRID_SIZE; i++) 
	{
        	allerxy(GRID_ORIGIN_X - 1, GRID_ORIGIN_Y + i);
        	printf("|                  |");
    	}

    	allerxy(GRID_ORIGIN_X - 1, GRID_ORIGIN_Y + GRID_SIZE);
    	printf("+==================+");
}

void draw_car(Vehicule *v) {
    int x, y;

    /* SCALE THE POSITION */
    int sx = GRID_ORIGIN_X + v->x;
    int sy = GRID_ORIGIN_Y + v->y;

    int len = (v->dir == 'h') ? v->length * CELL : CELL;
    int hei = (v->dir == 'v') ? v->length * CELL : CELL;

    couleurfond(v->color);

    for (y = 0; y < hei; y++) {
        allerxy(sx, sy + y);
        for (x = 0; x < len; x++)
            printf(" ");
    }

    resetcouleurs();
}





int main(void)
{
    nostdoutbuff();
    desactive_curseur();
    efface_ecran();

    draw_grid();

    Vehicule cars[] = {
        {0,  0,  0, 'v', 3, COLOR_GREEN},  // left vertical
        {1,  3,  6, 'h', 2, COLOR_RED},  // middle horizontal
        {2,  9,  0, 'v', 3, COLOR_BLUE},  // right vertical
        {3, 12,  6, 'h', 2, COLOR_CYAN},  // bottom horizontal
    };

    int n = sizeof(cars) / sizeof(cars[0]);
    for (int i = 0; i < n; i++)
        draw_car(&cars[i]);

    allerxy(1, 1);
    printf("Rendering test n°3 — press Ctrl+C to quit");

    int c;
    do{
	c = getchar();
      } while (c != 'q');

    resetterminal();
    active_curseur();
    return 0;
}
