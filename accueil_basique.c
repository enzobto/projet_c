#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int joueur(int n, char pseudo1[n],char pseudo2[n], int j){

    printf("                ------------------------------------------------------\n");
    printf("                |                                                    |\n");
    printf("                |                Bienvenue  dans                     |\n");
    printf("                |                                                    |\n");
    printf("                |                   POLYTOPIA                        |\n");
    printf("                |                                                    |\n");
    printf("                | Il y a 1 ou 2 joueurs ?                            |\n");
    printf("                ------------------------------------------------------\n");
    scanf("%d\n", &j);
    
    while((j != 1) && (j != 2)){

        if((j != 1) && (j != 2)){
        printf("Entrez 1 ou 2 !\n");
    }
        scanf("%d\n", &j);
        
    }

    if (j == 1){
        printf("Ton pseudo est : "); //probleme : la fonction demande pseudo1 AVANT d'executer cette ligne
        scanf("%s", pseudo1);
        printf("\n");
        pseudo2 = "IA";
    } else if (j == 2){
        printf("Le pseudo du joueur 1 est : ");
        scanf("%s", pseudo1);
        printf("\n");
        printf("Le pseudo du joueur 2 est : ");
        scanf("%s", pseudo2);
        printf("\n");
    }

    printf("                ------------------------------------------------------\n");
    printf("                | Salut %s, tu es prêt a jouer contre %s ?           |\n", pseudo1, pseudo2);
    //printf("                        |\n");
    printf("                |                                                    |\n");
    printf("                | Oui : entrez 'y'                                   |\n");
    printf("                | Non : entrez 'n'                                   |\n");
    printf("                ------------------------------------------------------\n");

    char c;
    c = 'a';

    while((c != 'y') && (c != 'n')){
    c = getchar(); // On attends que le joueur dise oui ou non
    if((c != 'y') && (c != 'n')){
        printf("Entrez 'y' ou 'n' !\n");
    }
    }

    if (c == 'y'){
        return 42;
    } else if (c == 'n'){
        return 12;
}
}

int main(void){
    char pseudo1[100] = {'\0'}; //Nom du premier joueur
    char pseudo2[100] = {'\0'}; // Nom du second joueur ou IA
    int j = 0; // le nb de joueurs
    if (joueur(100, pseudo1, pseudo2, j) == 42){
        printf("Normalement ca marche !\n");
        return EXIT_SUCCESS;
        // lancer le jeu
    } else {
        printf("Au plaisir de vous revoir !\n");
        return EXIT_SUCCESS;
        // ferme le jeu
    }

}
