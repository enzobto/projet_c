#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define LONGUEUR 14
#define LARGEUR 14


void initialise_map(int largeur, int longueur, char mat[largeur][longueur]) {
  for (int i = 0; i < largeur; i++){
    for (int j = 0; j < longueur; j++){ //rempli une map de #
      mat[i][j] ='#';
    }
  }
}

void affiche_map(int largeur, int longueur,char map[largeur][longueur]){
for(int i = 0; i < largeur; i++){
 for(int j = 0; j < longueur; j++){ //affiche la map
   printf("%c",map[i][j]);
 }
   printf("\n");
 }

}

void change_case(int largeur, int longueur, int abs, int ord, char element,char map[largeur][longueur]){
  if(longueur > abs && largeur > ord){    
    map[ord][abs] = element; //place un element sur la map
    }
  else{
    printf("On tente de placer l'élément en dehors du tableau (change_case)");
  }
}


int main(){
char map[LARGEUR][LONGUEUR] = {{'\0'}};
initialise_map(LARGEUR, LONGUEUR, map);
change_case(LARGEUR, LONGUEUR, 7, 7, '@', map);
affiche_map(LARGEUR, LONGUEUR, map);
}
