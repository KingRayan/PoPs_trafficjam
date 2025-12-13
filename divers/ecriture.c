#include <stdio.h>

int main(void) {
  FILE *fpsrc, *fpdest; /* création d'un pointeur sur fichier */
  int i;
  fpsrc = fopen("tab.txt", "r"); /* ouverture en lecture (r:read) */
  fpdest = fopen("tabcopie.txt", "w"); /* ouverture en écriture seule (w:write) */
                                       /* pour ajouter des éléments à la suite d'un
					* fichier, il faut utiliser a (a:append) à la
					* place de w
					*/
  if (fpsrc == NULL) { /* si le fichier n'existe pas fp = NULL, le pointeur ne pointe sur rien */
    printf("Erreur de lecture fichier\n");
    return -1;
  }
  i = 0;
  while (fscanf(fpsrc, "%d", &i) == 1)
    fprintf(fpdest, "%d ", i); /* écriture dans le fichier destination */
  fclose(fpsrc); /* on ferme le fichier source */
  fclose(fpdest); /* on ferme le fichier destination */
  return 0;
}
