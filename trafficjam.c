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


typedef struct {
	int id; 		// referencer les vehicules de la grille
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

/* =================== BASE DU JEU ====================== */

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
				grid[v->y + dy][v->x + dx] = v->id; // associer les cellules occupées aux véhicules distincts	
	}

}

/* ================== INIT ======================== */
Game init_game(void)
{
	Game s;
	s.count = 4;
	s.selected = 0;

	s.Cars[0] = (Vehicule){0, 3, 6, 'h', 2, 31};
	s.Cars[1] = (Vehicule){1, 0, 0, 'v', 3, 32};
	s.Cars[2] = (Vehicule){2, 9, 0, 'v', 3, 34};
	s.Cars[3] = (Vehicule){3, 12, 6, 'v', 2, 35};
	
	return s;
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

void draw_car(Vehicule *v) 
{
	int x, y;

    	/* SCALE THE POSITION */
    	int sx = GRID_ORIGIN_X + v->x;
    	int sy = GRID_ORIGIN_Y + v->y;

    	int len = (v->dir == 'h') ? v->length * CELL : CELL;
    	int hei = (v->dir == 'v') ? v->length * CELL : CELL;

    	couleurfond(v->color);

    	for (y = 0; y < hei; y++) 
	{
        	allerxy(sx, sy + y);
        	for (x = 0; x < len; x++)
            		printf(" ");
    	}

    	resetcouleurs();
}

void render(Game *s)
{
	int i;
	
	efface_ecran();
	draw_grid();

	for (i = 0; i < s->count; i++)
		draw_car(&s->Cars[i]);
	
	allerxy(1, 1);
    	printf("Selected car: %d  ", s->selected);

    	allerxy(1, 2);
    	printf("Orientation: %s      ", s->Cars[s->selected].dir == 'h'? "HORIZONTAL" : "VERTICAL");


    	allerxy(1, 5);
    	printf("Arrows: selection || ('h'/'j'/'k'/'l'): movement || 'q' / CRTL + C: quit");
}

int read_key(void) {
	int c = recupcar();
    	if (c == 27) 
	{
        	if (recupcar() == '[')
            		return recupcar();  
	}
    	return c;
}




/* =============== MOVEMENT =================== */
int can_move(Game *s, int id, int dx, int dy) {
    	int grid[GRID_SIZE][GRID_SIZE];
    	int x, y;

    	build_grid(grid, s);

    	Vehicule *v = &s->Cars[id];

    	if (v->dir == 'h' && dy != 0) return 0;
    	if (v->dir == 'v' && dx != 0) return 0;

    	int nx = v->x + dx;
   	int ny = v->y + dy;

    	int w = (v->dir == 'h') ? v->length * CELL : CELL;
    	int h = (v->dir == 'v') ? v->length * CELL : CELL;

    	if (nx < 0 || ny < 0 ||
        	nx + w > GRID_SIZE || ny + h > GRID_SIZE)
        	return 0;

    	for (y = 0; y < h; y++)
        	for (x = 0; x < w; x++) {
            	int g = grid[ny + y][nx + x];
            	if (g != -1 && g != id)
                	return 0;
        	}

	    return 1;
}

void move_vehicule(Game *s, int id, int dx, int dy)
{
	if(can_move(s, id, dx, dy))
	{
		s->Cars[id].x += dx;
		s->Cars[id].y += dy;
	}
}



int main(void)
{
	nostdoutbuff();
	desactive_curseur();

    	Game game = init_game();

   	while (1) 
    	{
        	render(&game);
		int key = read_key();

        	allerxy(1, 7);
        	printf("TEST n°6 : Affichage et mouvement");
        
		if(key == 'q') break;
	
		if(key == 'A')
			game.selected = (game.selected - 1 + game.count) % game.count;
	
		else if (key == 'B')
            		game.selected = (game.selected + 1) % game.count;

        	else if (key == 'h')
            		move_vehicule(&game, game.selected, -CELL, 0);

        	else if (key == 'l')
            		move_vehicule(&game, game.selected, CELL, 0);

        	else if (key == 'k')
            		move_vehicule(&game, game.selected, 0, -CELL);

        	else if (key == 'j')
            		move_vehicule(&game, game.selected, 0, CELL);
      	}

    	resetterminal();
    	active_curseur();
	return 0;
}

