#include <stdio.h>
#include <time.h>
#include "console.h"
#include <stdlib.h>


/* INFORMATIONS SUR LA GRID */
#define GRID_SIZE 6
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
	int moves;				// nb  moves in a game
	int HELP;
}Game;


const char *color_name(int color)
{
    	switch (color)
       	{
        	case 31: return "RED";
        	case 32: return "GREEN";
        	case 33: return "YELLOW";
        	case 34: return "BLUE";
        	case 35: return "MAGENTA";
        	case 36: return "CYAN";
        	case 37: return "WHITE";
        	default: return "UNKNOWN";
    	}
}

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
		int lar = (v->dir == 'h') ? v->length : 1;	// horizontal <=> length*CELL

		int lon = (v->dir == 'v') ? v->length : 1;	// vertical   <=> height*CELL
		
		for(dy = 0; dy < lon; dy++)
			for(dx = 0; dx < lar; dx++)
				grid[v->y + dy][v->x + dx] = v->id; // associer les cellules occupées aux véhicules distincts	
	}

}

int check_win(Game *s) 
{
    	Vehicule *red = &s->Cars[0];

    	if (red->dir != 'h')
        	return 0;

    	if (red->x + red->length == GRID_SIZE)
        	return 1;

    	return 0;
}


/* ================== INIT ======================== */
Game init_game(void) {
    	Game s;
    	s.count = 5;
    	s.selected = 0;
	s.moves = 0;
	s.HELP = 0;

    	// Red Car
    	s.Cars[0] = (Vehicule){0, 1, 2, 'h', 2, 31};

    	// Vertical Cars
    	s.Cars[1] = (Vehicule){1, 3, 0, 'v', 3, 32};
    	s.Cars[2] = (Vehicule){2, 4, 3, 'v', 2, 34};

    	// Horizontal Cars
    	s.Cars[3] = (Vehicule){3, 0, 0, 'h', 3, 35};
    	s.Cars[4] = (Vehicule){4, 2, 5, 'h', 2, 36};

    	return s;
}


/* ================== RENDERING ======================== */

void draw_grid(void)
{
    	int gx, gy;

    	int width  = GRID_SIZE * CELL;
    	int height = GRID_SIZE * CELL;

    	/* Horizontal lines */
    	for (gy = 0; gy <= GRID_SIZE; gy++) 
    	{
        	int sy = GRID_ORIGIN_Y + gy * CELL;

        	for (gx = 0; gx <= width; gx++) 
		{
            		allerxy(GRID_ORIGIN_X + gx, sy);

            		if (gx == 0 || gx == width)
                		printf("+");
            		else
                		printf("-");
        	}
    	}

    	/* Vertical lines */
    	for (gx = 0; gx <= GRID_SIZE; gx++) {
        	int sx = GRID_ORIGIN_X + gx * CELL;

        	for (gy = 0; gy <= height; gy++) 
		{
            		allerxy(sx, GRID_ORIGIN_Y + gy);

            		/* Avoid overwriting corners already drawn */
            		if (gy % CELL != 0) printf("|");
        	}
	}
}

void draw_exit(void)
{
	
    	int exit_y = GRID_ORIGIN_Y + ((GRID_SIZE / 2) - 1) * CELL;

    	int exit_x = GRID_ORIGIN_X + GRID_SIZE * CELL;

    	
    	for (int i = 0; i < CELL; i++) // rows (lignes)
    	{
        	allerxy(exit_x, exit_y + i);
        	for (int j = 0; j < CELL; j++) // columns (colonnes)
            		printf(" ");
    	}

}

void draw_car(Vehicule *v, int selected)
{
	int x, y;
    	int sx = GRID_ORIGIN_X + v->x * CELL + 1; // + 1 evite la barriere gauche
    	int sy = GRID_ORIGIN_Y + v->y * CELL + 1;

    	int len = ((v->dir == 'h') ? v->length : 1) * (CELL - 1); // laisse une colonne pour cote droit
    	int hei = ((v->dir == 'v') ? v->length : 1) * (CELL - 1); // laiise une ligne pour cote bas

    	if (selected) mode_clignotant();
    	couleurpolice(v->color);

    	for (y = 0; y < hei; y++)
    	{
        	allerxy(sx, sy + y);
        	for (x = 0; x < len; x++)
            		printf("█");
      	}
    	resetcouleurs();
    	if (selected) desactive_mode_clignotant();
}

void draw_HUD(Game *s)
{	
	int y = 1;
	Vehicule *v = &s->Cars[s->selected];

	// ID
	allerxy(1, y);
	printf("Selected car ID : %d", s->selected);
	
	// Couleur
	allerxy(1, ++y);
	printf("Color : \t");
	couleurpolice(v->color);
	printf("%s", color_name(v->color));
	couleurpolice(37); // reset (blanc)
	
	// Direction
	allerxy(1,++y);
	printf("Direction : %s", v->dir == 'h' ? "HORIZONTAL" : "VERTICAL");
	
	// Nb Moves
	allerxy(1,++y);
	printf("Moves : %d", s->moves);

	// help
	allerxy(1, y + 2);
	printf("Press Shift + H for controls");

}

void draw_controls(void)
{
	int y = 8;

    	allerxy(1, y++);
    	printf("=== CONTROLS ===");

    	allerxy(1, y++);
    	printf("Selection : Arrow UP / Arrow DOWN");

    	allerxy(1, y++);
    	printf("Movement  : h = left   l = right");

    	allerxy(1, y++);
    	printf("            k = up     j = down");

    	allerxy(1, y++);
    	printf("Toggle UI : Shift + H");

    	allerxy(1, y++);
    	printf("Quit      : q");
}
void win_screen(Game *s)
{
	efface_ecran();
	allerxy(GRID_ORIGIN_X + GRID_SIZE / 2, GRID_ORIGIN_Y + GRID_SIZE / 2);
	policegras();
	couleurfond(42);
	couleurpolice(32);
	printf("YOU WIN !");
	couleurpolice(37);
	allerxy(1, GRID_ORIGIN_Y + GRID_SIZE / 2 + 1);
	printf("Took you ");
	couleurpolice(31);
	printf("%d ", s->moves);
	couleurpolice(37);
	printf("moves");
	resetcouleurs();
	allerxy(1, GRID_ORIGIN_Y + GRID_SIZE +2);
	printf("Press any key to exit...");
	recupcar();
}

void render(Game *s)
{
	efface_ecran();
	draw_grid();
	draw_exit();
	
	for (int i = 0; i < s->count; i++)
		draw_car(&s->Cars[i], i == s->selected);
	
	draw_HUD(s);

	if(s->HELP) draw_controls();
    	
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

    	int w = (v->dir == 'h') ? v->length : 1;
    	int h = (v->dir == 'v') ? v->length : 1;

    	if (nx < 0 || ny < 0 || nx + w > GRID_SIZE || ny + h > GRID_SIZE)
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
		s->moves += 1;
	}

}
/* =================== INPUT ==================== */
int read_key(void) {
        int c = recupcar();
        if (c == 27) 
        {
                if (recupcar() == '[')
                        return recupcar();
        }
        return c;
}



/* ================== MAIN ===================== */
int main(void)
{
	nostdoutbuff();
	desactive_curseur();

    	Game game = init_game();

   	while (1) 
    	{
        	int won = 0;
		render(&game);
		
		if(check_win(&game))
		{
			win_screen(&game);
			break;
		}

		int key = read_key();
       
		if(key == 'q') break;

		if(key == 'H') game.HELP = !game.HELP; // VHDL 101
	
		if(key == 'B')
			game.selected = (game.selected - 1 + game.count) % game.count;
	
		else if (key == 'A')
            		game.selected = (game.selected + 1) % game.count;

        	else if (key == 'h')
            		move_vehicule(&game, game.selected, -1, 0);

        	else if (key == 'l')
            		move_vehicule(&game, game.selected, 1, 0);

        	else if (key == 'k')
            		move_vehicule(&game, game.selected, 0, -1);

        	else if (key == 'j')
            		move_vehicule(&game, game.selected, 0, 1);
      	}

	resetterminal();
    	active_curseur();
	return 0;
}

