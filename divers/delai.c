#include <stdio.h> 
#include <time.h> 

void delai(int nbsecondes) { 
  /* Calcul: chaque système définit le nombre de périodes
   * d'horloge par seconde
   * La fonction clock() renvoie le temps en nombre de périodes
   * d'horloge (ou clock ticks)
   */
  int pause = nbsecondes*CLOCKS_PER_SEC;

  /* Mémorisation de l'heure de départ */
  clock_t start_time = clock(); 

  /* On boucle tant que la temporisation n'est pas atteinte */
  while (clock() < start_time + pause); 
}

int main(void) {
  int i;
  for (i = 0; i < 10; i++) {
    /* delai d'une seconde */
    delai(1);
    printf("%d secondes se sont écoulées\n", i + 1);
  }
  return 0;
}
