#include <stdio.h>

#define fichier "tab.txt"
#define NB_ELEM_MAXI 10

int main(void) {

  FILE *fp; /* création d'un pointeur sur fichier */
  int t[NB_ELEM_MAXI], i, j;
  fp = fopen(fichier, "r"); /* ouverture en lecture (r:read) */
  if (fp == NULL) { /* si le fichier n'existe pas fp = NULL, le pointeur ne pointe sur rien */
    printf("Erreur de lecture fichier\n");
    return -1;
  }
  i = 0;
  while (fscanf(fp, "%d", &t[i]) == 1) /* fscanf lit un élément du fichier et renvoie 1 en cas de succès
				        * après chaque fscanf réussi, le pointeur de fichier passe
				        * à l'élément suivant présent dans le fichier
				        */
    i++;
  fclose(fp); /* on ferme le fichier */
  for(j=0;j<i;j++)
    printf("%d\n",t[j]);
  return 0;
}
