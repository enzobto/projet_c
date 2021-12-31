#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

int accueil(void){

    clear();
    printf(
      "                ------------------------------------------------------\n"
      "                |                                                    |\n"
      "                |                Bienvenue  dans                     |\n"
      "                |                                                    |\n"
      "                |                   POLYTOPIA                        |\n"
      "                |                                                    |\n"
      "                | Ce jeux se joue à deux, vous êtes prêt ?           |\n"
      "                |                                                    |\n"
      "                |  Oui : entrez 'y'                                  |\n"
      "                |  Non : entrez 'n'                                  |\n"
      "                |                                                    |\n"
      "                ------------------------------------------------------\n"
    );
    printf("\n");
    gotoxy(20, 11);
    char c;
    c = 'a';

    while((c != 'y') && (c != 'n')){
    c = getchar(); // On attends que le joueur dise oui ou non
    if((c != 'y') && (c != 'n')){
        printf("Entrez 'y' ou 'n' !\n");
    }
    }

    if (c == 'y'){
      printf("\nBon jeux à vous\n");
        return 42;
    } else if (c == 'n'){
        printf("\nÀ bientôt\n");
        return 12;
}
  return 0;
}

int main(void){

    accueil();

}
