#include <stdio.h>
#include <time.h>
#include "console.h"
#include <stdlib.h>

/**
 * @author : Rayan BEN,
 * Projet PoPs_trafficjam
 * Dec 2025 - Jan 2026
 */

/* INFORMATIONS SUR LA GRID */

#define GRID_SIZE 6  				/* taille logique de la grille (6x6) */
#define CELL 3					/* cellule logique (3x3) {rendering} */
#define MAX_VEHICULES 10			/* nb max vehicules par partie */

#define GRID_ORIGIN_X 32	
#define GRID_ORIGIN_Y 15			/* grille <=> repere orthonormé vers la droite et le bas */


typedef struct {
	int id; 				/* referencer les vehicules de la grille */
	int x, y; 				/* top-left point vehicule */
	char dir; 				/* 'h'(horizontal) or 'v' (vertical) */
	int length; 				/* 2(car) or 3(truck) */
	int color; 				/* differencier les voitures coté UX */
}Vehicule;


typedef struct{
	Vehicule Cars[MAX_VEHICULES]; 		/* vehicules de la grille */ 
	int count; 		     		/* nb vehicules actifs */
	int selected;				/* id vehicule selected */
	int moves;				/* nb  moves in a game */
	int HELP;				/* bool d'affichage d'aide (UX) */
}Game;


/**
 * @fonction : Retourne le nom lisible d'une couleur ANSI à partir de son code.
 *
 * Utilisée uniquement pour l'affichage dans le HUD  **stockée en read-only car non-modifiable**
 * -> Rendre les informations plus compréhensibles pour le user.
 *
 * @param 	color 		Code_ANSI_de_la_couleur
 * @return 	const char*	Nom_de_la_couleur ou "UNKNOWN" si non reconnue
 */
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

/* ================================================ BASE DU JEU ================================================ */

/**
 * @fonction : Construit une représentation logique de la grille du jeu.
 *
 * Remplit une grille 6x6 avec :
 *  	-1 pour les cases vides
 *  	ID du véhicule présent dans la CELL
 *
 * Utilisée pour :
 * 	Detection de collisions
 *  	Validation des déplacements
 *
 * @param 	grid 		Tableau 2D (grille logique)
 * @param 	s 		Pointeur vers l'état actuel du jeu
 * @return 	//
 */
void build_grid(int grid[GRID_SIZE][GRID_SIZE], Game *s)
{
	int x, y; 							/* iteration sur la grille (x, y) */
	int i;	  							/* iteration sur les vehicules */
	int dx, dy;							/* iteration sur un vehicule(dx, dy) */

	/* initialiser la grille : cases vides */
	for(y = 0; y < GRID_SIZE; y++)
		for(x = 0; x < GRID_SIZE; x++) 
			grid[y][x] = -1; 				/* -1 = empty */
	
	/* placer les vehicules sur la grille */
	for(i = 0; i < s->count; i++)
	{
		Vehicule *v = &s->Cars[i]; 				/* pointe sur le vehicule actuel */
		
		/* fixer sa taille (en fonction de x et y) */
		int len = (v->dir == 'h') ? v->length : 1;		/* longueur logique : x */
		int hei = (v->dir == 'v') ? v->length : 1;		/* hauteur logique  : y */
		
		/* associer les cases aux vehicules qui les occupent */		
		for(dy = 0; dy < hei; dy++)
			for(dx = 0; dx < len; dx++)
				grid[v->y + dy][v->x + dx] = v->id; 	/* mettre ID vehicule dans chaque case qu'il occupe */
	}

}

/**
 * @fonction : Vérifie condition de victoire.
 *
 * Victoire ssi:
 * 	Voiture rouge (id 0)
 * 	|-----> est horizontale
 * 	|----->	atteint la sortie à droite de la grille
 *
 * @param 	s 		Pointeur vers l'état actuel du jeu
 * @return 	int 		1 si victoire, 0 sinon 			{agit comme un bool}
 */
int check_win(Game *s) 
{
    	Vehicule *red = &s->Cars[0]; 					/* annoncer redCar a ID 0 */
	
	/* redCar ne peut pas etre en pos vertical */
    	if (red->dir != 'h')
        	return 0;						/* err si redCar vertical */

	/* redCar reached exit */
    	if (red->x + red->length == GRID_SIZE)				
        	return 1; 						/* win */

    	return 0;
}


/* ================================================== INIT ==================================================== */

/**
 * @fonction : Initialise une partie de trafficjam.
 *
 * Fixer un état de jeu cohérent :
 * 	initialise les variables de jeu 
 * 	definit les voitures
 * 			|--> ID 
 * 			|--> positions initiales (x, y)
 * 			|--> directions
 * 			|--> longueurs
 * 			|--> couleurs
 *
 * @return 	Game 		Structure représentant l'état de la partie
 */
Game init_game(void) 
{
	/* initialiser la partie */
    	Game s;

	/* variables de jeu */
    	s.count = 5; 							/* nb vehicules de la partie */
    	s.selected = 0;							/* ID vehicule courant (initial redCar) */
	s.moves = 0;							/* aucun move initial */
	s.HELP = 0;							/* aide desactivee par defaut (toggle) */

    	/* Red Car */
    	s.Cars[0].id = 0;
	s.Cars[0].x = 1;
	s.Cars[0].y = 2;
	s.Cars[0].dir = 'h';
	s.Cars[0].length = 2;
	s.Cars[0].color = 31;



    	/* Vertical Cars */
    	s.Cars[1].id =1;
    	s.Cars[1].x = 3;
    	s.Cars[1].y = 0;
    	s.Cars[1].dir = 'v';
    	s.Cars[1].length = 3;
    	s.Cars[1].color = 32;

    	s.Cars[2].id = 2;
    	s.Cars[2].x = 4;
    	s.Cars[2].y = 3;
    	s.Cars[2].dir = 'v';
    	s.Cars[2].length = 2;
    	s.Cars[2].color = 34;

    	/* Horizontal Cars */
    	s.Cars[3].id = 3;
    	s.Cars[3].x = 0;
    	s.Cars[3].y = 0;
    	s.Cars[3].dir = 'h';
    	s.Cars[3].length = 3;
    	s.Cars[3].color = 35;

	s.Cars[4].id = 4;
	s.Cars[4].x = 2;
	s.Cars[4].y = 5;
	s.Cars[4].dir = 'h';
	s.Cars[4].length = 2;
	s.Cars[4].color = 36;

    	return s;
}


/* ==================================================== RENDERING ============================================================ */

/**
 * @fonction : Affiche la grille on screen
 *
 * Cette fonction affiche une grille 6x6 mise à l'échelle
 * par le facteur CELL (cellule logique) 
 *
 * Purement graphique
 * 	pas d'effet de collision
 * 	pas d'information sur les véhicules
 *
 * Coordonnées logiques (6x6) sont transformées
 * en coordonnées écran via CELL.
 *
 * @return 	void
 */
void draw_grid(void)
{
    	int gx, gy;

    	int width  = GRID_SIZE * CELL;						/* width on screen (nb caracteres) */
    	int height = GRID_SIZE * CELL;						/* height on screen (nb caracteres) */

    	/* ================ LIGNES HORIZONTALES ================== */
    	
	for (gy = 0; gy <= GRID_SIZE; gy++) 					/* iteration sur les lignes grid */
    	{
        	int sy = GRID_ORIGIN_Y + gy * CELL;				/* calcul equivalent grid_y --> screen_y */

        	for (gx = 0; gx <= width; gx++) 				/* iteration sur les colonnes grid */
		{
            		allerxy(GRID_ORIGIN_X + gx, sy);			/* va a chaque point de cellules */

            		if (gx == 0 || gx == width)				/* verifie coins et bordures */
                		printf("+");					/* + aux coins des cellules */
            		else
                		printf("-");					/* - autrement */
        	}
    	}

      	/* ================ LIGNES VERTICALES ================== */

    	for (gx = 0; gx <= GRID_SIZE; gx++) 					/* iteration sur les colonnes grid */
	{
        	int sx = GRID_ORIGIN_X + gx * CELL;				/* calcul equivalent grid_x --> screen_x */
				
        	for (gy = 0; gy <= height; gy++)				/* iteration sur les lignes grid */
		{
            		allerxy(sx, GRID_ORIGIN_Y + gy);			/* va a chaque point de cellule */

            		/* Modulo cell pour savoir positionnement */
            		if (gy % CELL != 0) printf("|");			/* tracer '|' si modulo non nul, autrement c'est un coin donc '+' deja oresent */
        	}
	}
}

/**
 * @fonction : Dessine la sortie de la grille.
 *
 * Sortie sur le bord droit de la grid,
 * Ligne centrale (voiture rouge).
 *
 * Representee par un espace vide de hauteur CELL.
 *
 * @return 	void
 */
void draw_exit(void)
{
	int i, j;
	/* position verticale (y) de la sortie */
    	int exit_y = GRID_ORIGIN_Y + ((GRID_SIZE / 2) - 1) * CELL;		/* milieu */
	
	/* position horizontale (x) de la sortie */
    	int exit_x = GRID_ORIGIN_X + GRID_SIZE * CELL;				/* fin de grille */

    	
    	for (i = 0; i < CELL; i++) 						/* itération sur les rows (lignes) */
    	{
        	allerxy(exit_x, exit_y + i);
        	for (j = 0; j < CELL; j++) 					/* itération sur les columns (colonnes) */
            		printf(" ");						/* VISUELLEMENT trace l'ouverture de sortie */
    	}

}

/**
 * @fonction : Dessine un vehicule à l'ecran.
 *
 * Affiche le vehicule en utilisant le char '█'
 * 	
 * ----> Si le véhicule est sélectionné, il clignote.
 *
 * @param 	v 		Pointeur vers le véhicule à dessiner
 * @param 	selected 	1 si le véhicule est sélectionné, 0 sinon
 */
void draw_car(Vehicule *v, int selected)
{
	int x, y;

	/* position on-screen (TopLeft point du vehicule) */
    	int sx = GRID_ORIGIN_X + v->x * CELL + 1; 			/* + 1 evite la CellBorder gauche */
    	int sy = GRID_ORIGIN_Y + v->y * CELL + 1;			/* +1 evite la CellBorder haute */
	
	/* taille on-screen du vehicule */
    	int len = ((v->dir == 'h') ? v->length : 1) * (CELL - 1); 	/* laisse une colonne pour CellBorder droit */
    	int hei = ((v->dir == 'v') ? v->length : 1) * (CELL - 1); 	/* laiise une ligne pour CellBorder bas */

    	/* clignote si le vehicule est selected */
	if (selected) mode_clignotant();
    	couleurpolice(v->color);
	
	/* dessin char par char */
    	for (y = 0; y < hei; y++)					/* iteration sur hauteur (y) du vehicule */
    	{
        	allerxy(sx, sy + y);
        	for (x = 0; x < len; x++)				/* iteration sur longueur (x) du vehicule */
            		printf("█");
      	}

	/* reset */ 
    	resetcouleurs();
    	if (selected) desactive_mode_clignotant();
}

/**
 * @fonction : Affiche des informations sur l'etat actuel d'une partie (HUD)
 *
 * Affiche :
 * 	|-----> ID du véhicule sélectionné
 *  	|-----> Couleur
 *  	|----->	Direction
 *  	|----->	Nombre de mouvements
 *  	|----->	Aide rapide
 *
 * @param 	s 		Pointeur vers l'etat actuel de la partie
 */
void draw_HUD(Game *s)
{	
	int y = 1;									/* lignes on-screen */

	Vehicule *v = &s->Cars[s->selected];						/* faire pointer v sur le vehicule courant */

	/* Affiche ID du vehicule courant */
	allerxy(1, y);						
	printf("Selected car ID : %d", s->selected);					/* affiche ID vehicule courant */
	
	/* Affiche couleur du vehicule dans sa meme couleur */
	allerxy(1, ++y);	
	printf("Color : \t");
	couleurpolice(v->color);							/* couleur texte vehicule courant */
	printf("%s", color_name(v->color));						/* appel a LUT et affiche couleur vehicule courant */
	couleurpolice(37); 								/* couleur texte reset */ 
	
	/* Affiche direction du vehicule courant */
	allerxy(1,++y);
	printf("Direction : %s", v->dir == 'h' ? "HORIZONTAL" : "VERTICAL");		/* affiche si vehicule Horizontal ou Vertical */
	
	/* Affiche le nombre de mouvements effectués au courant de la partie */
	allerxy(1,++y);
	printf("Moves : %d", s->moves);							/* affiche nb moves */ 

	/* Affiche le texte pour indiquer comment afficher l'aide */
	allerxy(1, y + 2);
	printf("Press Shift + H for controls");						/* affiche txt help */

}

/**
 * @fonction : Affiche les contrôles du jeu à l'écran (HELP).
 *
 * Affiche les touches pour :
 * 	|---> Sélectionner une voiture
 * 	|---> Faire bouger une voiture
 * 	|---> Toggle UI
 * 	|---> Quitter le jeu
 *
 * @return 	void
 */
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

/**
 * @fonction : Affiche l'ecran de victoire (Win Screen)
 *
 * Efface l'ecran et affiche un message "YOU WIN !" au centre
 * de la grille avec le nombre de mouvements total.
 *
 * --> Utilise Couleurs Texte Gras pour l'effet visuel.
 *
 * @param 	s 		Pointeur vers l'état du jeu
 * @return	void
 */
void win_screen(Game *s)
{
	efface_ecran();						/* clear screen */
	
	/* Affiche YOU WIN au milieu de l'ecran */
	allerxy(GRID_ORIGIN_X + GRID_SIZE / 2, GRID_ORIGIN_Y + GRID_SIZE / 2);
	policegras();
	couleurpolice(32);					/* COLOR TEXTE GREEN */
	printf("YOU WIN !");
	
	/* Affiche le nombre de moves */
	couleurpolice(37);
	allerxy(1, GRID_ORIGIN_Y + GRID_SIZE / 2 + 1);
	printf("Took you ");
	couleurpolice(31);					/* COLOR TEXT WHITE */
	printf("%d ", s->moves);
	couleurpolice(37);					/* COLOR TEXT RED */
	printf("moves");
	resetcouleurs();					/* RESET TEXT COLOR */
	
	/* Affiche le texte d'exit */
	allerxy(1, GRID_ORIGIN_Y + GRID_SIZE +2);
	printf("Press any key to exit...");
	recupcar();
}

/**
 * @fonction : Rendering etat du jeu {COMPLET} l'écran.
 *
 * Affiche : 
 * 	|-----> la grid
 * 	|-----> la sortie
 * 	|-----> les vehicules
 * 	|-----> le HUD
 * 	|-----> les controls si aide est activée.
 *
 * @param 	s 		Pointeur vers l'état du jeu
 * @return 	void
 */
void render(Game *s)
{	
	int i;
	efface_ecran();						/* clear screen */

	draw_grid();						/* affiche la grid */
	draw_exit();						/* affiche la sortie */
	
	for (i = 0; i < s->count; i++)
		draw_car(&s->Cars[i], i == s->selected);	/* affiche chaque voiture (itération boucle) */
								/* clignote si ID voiture qu'il dessine est celui selected */
	
	draw_HUD(s);						/* affiche le HUD */

	if(s->HELP) draw_controls();				/* affiche les controls si activated */
    	
}

/* ====================================== MOVEMENT =============================================== */

/**
 * @fonction : Verifie si un vehicule peut move dans une direction donnée.
 *
 * Cette fonction teste :
 *  	|---> Si le vehicule move dans sa direction
 *  	|---> Si le deplacement reste à l'interieur de la grid
 *  	|---> S'il n'y a pas de collision inter-vehicules
 *
 * @param 	s 		Pointeur vers l'etat de la partie (Game *)
 * @param 	id 		ID du véhicule à déplacer
 * @param 	dx 		Déplacement horizontal (-1 gauche, +1 droite)
 * @param 	dy 		Déplacement vertical (-1 haut, +1 bas)
 * @return 	int 		1 si le déplacement est possible, 0 sinon
 */
int can_move(Game *s, int id, int dx, int dy) 
{
    	int grid[GRID_SIZE][GRID_SIZE];						/* initialise une grid */
    	int x, y;

    	build_grid(grid, s);							/* build la grid de la partie */

    	Vehicule *v = &s->Cars[id];						/* fait pointer v sur le vehicule courant */
	
	/* teste les breakcase directionnels */
    	if (v->dir == 'h' && dy != 0) return 0;					/* direction contraire au mouvement */			
    	if (v->dir == 'v' && dx != 0) return 0;					/* donc non valide */
	
	/* calcul des nouvelles positions du vehicule */
    	int nx = v->x + dx;							
   	int ny = v->y + dy;

	/* fixe la taille du vehicule hauteur (y) et largeur (x) */
    	int w = (v->dir == 'h') ? v->length : 1;				
    	int h = (v->dir == 'v') ? v->length : 1;

    	/* teste les limites de la grid */
	if (nx < 0 || ny < 0 || nx + w > GRID_SIZE || ny + h > GRID_SIZE)
        	return 0;
	
	/* verif les collisions inter-vehicules */
    	for (y = 0; y < h; y++)
        	for (x = 0; x < w; x++) 
		{
            		int g = grid[ny + y][nx + x];					/* cellule a la nouvelle position */
            		if (g != -1 && g != id)						/* cellule non-vide <=> occupee par autre vehicule */
                		return 0;
        	}

	    return 1;
}

/**
 * @fonction : Move un vehicule ssi le move est valide.
 *
 * --> Appelle can_move() pour verifier la validité.
 * Si valide :
 *  	|---> Met à jour les coordonnées du véhicule
 *  	|---> Incrémente le compteur de mouvements du jeu
 *
 * @param 	s 		Pointeur vers l'etat de la partie (Game *)
 * @param 	id 		ID du véhicule à déplacer
 * @param 	dx 		Move horizontal (-1 gauche, +1 droite)
 * @param 	dy 		Move vertical (-1 haut, +1 bas)
 */
void move_vehicule(Game *s, int id, int dx, int dy)
{
	if(can_move(s, id, dx, dy))
	{
		s->Cars[id].x += dx;
		s->Cars[id].y += dy;
		s->moves += 1;								/* incremente le cpt moves de la partie */
	}

}

/* ================================================ INPUT ================================================ */

/**
 * @fonction : Lit une touche du clavier.
 *
 * Récupère une touche en mode non-bloquant
 * Gère les séquences d'échappement (Escape Sequence) pour les touches de selection (UpArrow, DownArrow).
 * esc seq : i.e "27 [ A" remplacer (A/B/C/D) en fonction de fleche appuyee
 * A haut - B Bas - C droite - D gauche
 * @return 	int 			code ASCII de la touche appuyée
 */
int read_key(void) 
{
        int c = recupcar();	 					/* rcupere le character */
        if (c == 27) 							/* detecte esc */
        {
                if (recupcar() == '[')					/* detecte esc sequence */
                        return recupcar();				/* lit le dernier char de la sequence */ 
	}
        return c;							/* retourne ce dernier char */
}

/* ================================================= MAIN ==================================================== */

/**
 * @brief Boucle principale du jeu.
 *
 * Initialise le jeu, désactive le curseur et le buffering,
 * puis lance la boucle principale qui :
 *  - rend l'état actuel de la grille et HUD
 *  - lit les touches appuyées
 *  - déplace les véhicules si possible
 *  - vérifie la victoire
 *
 * @return int Retourne 0 à la fin du programme
 */
int main(void)
{
	nostdoutbuff();									/* Visualiser les changements en temps reel */
											/* LIEN <=> électronique embarquée à temps critique */
                       									/* où chaque action doit être visible / traitée sans délai */
	desactive_curseur();

    	Game game = init_game();							/* initialise l'etat du jeu avec les vehicules */

   	while (1) 
    	{
		render(&game);								/* appel a render qui dessine grille, vehicules, HUD, Controls (si activated) */
		
		/* Verifie si game won */
		if(check_win(&game))					
		{
			win_screen(&game);						/* affiche le win screen */
			break;								/* sort de la boucle principale */
		}

		int key = read_key();							/* lit l'input du joueur en traitant les sequences */
       
		if(key == 'q') break;							/* quitter le jeu */

		if(key == 'H') game.HELP = !game.HELP; 					/* afficher les controls {VHDL 101 sig = !sig sur les testbench} */
		
		/* Selection du vehicule avec escape sequence (read_key marche) */ 
		if(key == 'B')
			game.selected = (game.selected - 1 + game.count) % game.count;	/* DownArrow */
	
		else if (key == 'A')
            		game.selected = (game.selected + 1) % game.count;		/* UpArrow */
		
		/* Move du vehicule*/
        	else if (key == 'h')
            		move_vehicule(&game, game.selected, -1, 0);			/* 'h' left */

        	else if (key == 'l')
            		move_vehicule(&game, game.selected, 1, 0);			/* 'l' right */

        	else if (key == 'k')
            		move_vehicule(&game, game.selected, 0, -1);			/* 'k' down */

        	else if (key == 'j')
            		move_vehicule(&game, game.selected, 0, 1);			/* 'j' up */
      	}

	resetterminal();
    	active_curseur();
	return 0;
}

