#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define square(a) (a)*(a)

#define GRASS     ' '
#define WATER     '~'
#define MOUNTAIN  '^'
#define TROUPE_R      '&'
#define TROUPE_B    '$'
#define CURSEUR    'O'
#define BLUE_CITY     'X'
#define RED_CITY     'x'
#define MOVE_ZONE '.'

#define GRASS_PAIR     1
#define WATER_PAIR     2
#define MOUNTAIN_PAIR  3
#define PLAYER_PAIR    4
#define RED_PAIR     5
#define BLUE_PAIR   6
#define MAGENTA_PAIR 7

//position des villes
#define XV1B 50
#define YV1B 30
#define XV2B 55
#define YV2B 15

#define XV1R 49
#define YV1R 18
#define XV2R 20
#define YV2R 30
/*
    int ordV1R = 18, absV1R = 49;
    int ordV2R = 30, absV2R = 10;
    int ordV1B = 30, absV1B = 200;
    int ordV2B = 15, absV2B = 170;
*/
/* int is_move_okay(int y, int x); */ 

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

int jeux();

void accueil(void){
    clear();
    printf("\033[H\033[J");
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
        jeux();
    } else if (c == 'n'){
        printf("\nÀ bientôt\n");
        endwin();
}
}

void draw_map(void);
int where(int y, int x);

//cette fonction réinitialise les affichages des ranges des troupes à chaques troupes
void reset_range_display(int mat_troupes[400][400][4]){
for(int i = 0; i < 400; i++){
            for(int j = 0; j < 400; j++){
                mat_troupes[i][j][3] = 0;
            }
        }
}


void affiche_tour(char tour){
    if(tour == 'b'){
        attron(COLOR_PAIR(BLUE_PAIR));          //affiche en haut a gauche l'équipe qui doit jouer

        char affiche_tour[100]="Au tour des Bleus~";
        int taille = strlen(affiche_tour);

        for(int i = 0; i < taille; i++){
            mvaddch(3, i + 3, affiche_tour[i]);
        }

        attroff(COLOR_PAIR(BLUE_PAIR));
        }

    else{
        attron(COLOR_PAIR(RED_PAIR));

        char affiche_tour[100]="Au tour des Rouges";
        int taille = strlen(affiche_tour);

        for(int i = 0; i < taille; i++){
            attron(COLOR_PAIR(RED_PAIR));
            mvaddch(3, i + 3, affiche_tour[i]);
        }
        attroff(COLOR_PAIR(RED_PAIR));
        }
}

int nbvilles(int villes[6][2]){
    int nbvilles = 0;
    for(int i = 0; i < 6; i++){
        if(villes[i][0] != -1){
            nbvilles++;
        }
    }
return nbvilles;
}

//Cette fonction indique si les coordonnées passées en arguments sont celles d'une ville
bool est_pos_ville(int x, int y, int villes[6][2]){
    for(int i = 0; i < 6; i++){
        if(villes[i][0] == x && villes[i][1] == y){
            return true;
        }
    }
    return false;
}

//Indique si la position en argument est celle d'une troupe
bool est_pos_troupe(int x,int y,int mat_troupes[400][400][4],int troupe){
    if(mat_troupes[y][x][0] == troupe){
        return true;
    }
    else{
        return false;  
    }}

void reinitialise_tour(int mat_troupes[400][400][4]){
        for(int i = 0; i < 400; i++){
            for(int j = 0; j < 400; j++){
                mat_troupes[i][j][2] = 0;
            }
        }
}

void affiche_mat(int mat_troupes[400][400][4]){
    for(int i = 0; i < 400; i++){
        for(int j = 0; j < 400; j++){
            printf("%d", mat_troupes[i][j][2]);
        }
    }
    printf("\n");
}

void modify_information(char info[100], char nouv[100]){
    for(int i = 0;i < 200; i++){
        info[i] = nouv[i];
    }

}


//Affiche la range de la troupe
void display_move_zone(int x,int y,int mat_troupes[400][400][4], int villesR[6][2], int villesB[6][2]){
    for(int ord = - 5; ord <= 5; ord++){
        for(int abs = - 10; abs <= 10; abs++){
            if(!est_pos_ville(x + abs ,y + ord, villesR) && !est_pos_ville(x + abs ,y + ord, villesB) && !est_pos_troupe(x + abs, y + ord, mat_troupes, 2) && !est_pos_troupe(x + abs, y + ord, mat_troupes, 1) && !((mvinch(y + ord, x + abs) & A_CHARTEXT) == WATER)){
                attron(COLOR_PAIR(MAGENTA_PAIR));
                mvaddch(y + ord, x + abs, MOVE_ZONE);
                attroff(COLOR_PAIR(MAGENTA_PAIR));
        }    
    }
}
}
//affiche des info en haut de l'écran
void display_information(char info[100],int lignes){
        attron(COLOR_PAIR(MOUNTAIN_PAIR));          //affiche en haut a gauche l'équipe qui doit jouer
        int taille = strlen(info);
        for(int i = 0; i < taille; i++){
            mvaddch(3, i + (lignes / 2) - (taille / 2), info[i]);
        }
        attroff(COLOR_PAIR(MOUNTAIN_PAIR));
        }

//Efface les info en haut de l'écran
void erase_information(char info[100],int lignes){
        attron(COLOR_PAIR(WATER_PAIR));          //affiche en haut a gauche l'équipe qui doit jouer
        int taille = strlen(info);
        for(int i = 0; i < 100; i++){
            mvaddch(3, i + (lignes / 2) - (taille / 2) - 20, WATER);
        }
        attroff(COLOR_PAIR(WATER_PAIR));
        }

void erase_move_zone(int x,int y,int mat_troupes[400][400][4], int villesR[2][2], int villesB[2][2]){
    for(int ord = - 5; ord <= 5; ord++){
        for(int abs = - 10; abs <= 10; abs++){
            if(!est_pos_ville(x + abs ,y + ord, villesR) && !est_pos_ville(x + abs ,y + ord, villesB) && !est_pos_troupe(x + abs, y + ord, mat_troupes, 2) && !est_pos_troupe(x + abs, y + ord, mat_troupes, 1) && !((mvinch(y + ord, x + abs) & A_CHARTEXT) == WATER)){
                attron(COLOR_PAIR(GRASS_PAIR));
                mvaddch(y + ord, x + abs, GRASS);
                attroff(COLOR_PAIR(GRASS_PAIR));
            }    
        }
    }
}

//Cette fonction dit si les coordonnées rentrées en arguments sont dans la range d'une troupe
bool is_move_zone(int x, int y, int xTroupe, int yTroupe, int mat_troupes[400][400][4], int troupe){
    if(((mvinch(y, x) & A_CHARTEXT) != WATER) && square(y - yTroupe) <= 30 && (square(x - xTroupe)) <= 120 && !est_pos_troupe(x, y, mat_troupes, troupe)){
        return true;
    }
    else{
        return false;
    }
}

//Cette fonction indique si la range d'une troupe est déjà affichée
bool is_range_display(int mat_troupes[400][400][4]){
    for(int i = 0; i < 400; i++){
        for(int j = 0; j < 400; j++){
            if(mat_troupes[i][j][3] == 1){
                return true;
            }
        }
    }
    return false;
}

//Ces deux fonctions retournent l'abs et l'ord de la troupe dont la range est affichée
int xtroupe_dep(int mat_troupes[400][400][4]){
    int abs = -1;
    for(int i = 0; i < 400; i++){
        for(int j = 0; j < 400; j++){
            if(mat_troupes[i][j][3] == 1){
                abs = j;
            }
        }
    }
    return abs;
}
int ytroupe_dep(int mat_troupes[400][400][4]){
    int ord = -1;
    for(int i = 0; i < 400; i++){
        for(int j = 0; j < 400; j++){
            if(mat_troupes[i][j][3] == 1){
                ord = i;
            }
        }
    }
    return ord;
}

void lose_city(int x,int y,int villes[6][2]){
    for(int i = 0; i < 6; i++){
        if(villes[i][0] == x && villes[i][1] == y){
            villes[i][0] = -1;
            villes[i][1] = -1;
        }
    }
}

void win_city(int x,int y, int villes[6][2]){
    for(int i = 0; i < 6; i++){
        if(villes[i][0] == -1 && villes[i][1] == -1){
            villes[i][0] = x;
            villes[i][1] = y;
        }
    }
}


int jeux()
{

        int y, x;
        int ch;
        char tour = 'b';
        char troupe_sous_curseur = '\0';;
        int nbTroupesRouge = 0;
        int nbTroupesBleu = 0;
        char ville_sous_curseur;

        //coordonnées des villes
        int ordV1R = YV1R, absV1R = XV1R;
        int ordV2R = YV2R, absV2R = XV2R;
        int ordV1B = YV1B, absV1B = XV1B;
        int ordV2B = YV2B, absV2B = XV2B;
        int villes_rouges[6][2] = {{absV1R,ordV1R},{absV2R,ordV2R},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
        int villes_bleus[6][2] = {{absV1B,ordV1B},{absV2B,ordV2B},{-1,-1},{-1,-1},{-1,-1},{-1,-1}}; 


        // initialize curses
        initscr();
        keypad(stdscr, TRUE);
        cbreak();
        noecho();

        // initialize colors
        start_color();
        init_pair(GRASS_PAIR, COLOR_WHITE, COLOR_GREEN);
        init_pair(WATER_PAIR, COLOR_CYAN, COLOR_BLUE);
        init_pair(MOUNTAIN_PAIR, COLOR_BLACK, COLOR_WHITE);
        init_pair(PLAYER_PAIR, COLOR_RED, COLOR_MAGENTA);
        init_pair(RED_PAIR, COLOR_WHITE, COLOR_RED);
        init_pair(BLUE_PAIR, COLOR_WHITE, COLOR_BLUE);
        init_pair(MAGENTA_PAIR,COLOR_WHITE, COLOR_MAGENTA);

        clear();

        // initialise la map
        draw_map();

        
        y = LINES / 2;
        x = COLS / 2;


        /* On crée une mémoire pour rechanger la couleur et le caractère en dessous */
        char mem_char = GRASS;
        int mem_color = GRASS_PAIR;
        int fut_color = GRASS_PAIR;
        char info[200];



        //on crée une matrice de la map sur laquelle on va stocker à l'emplacement d'une troupe : l'équipes à laquelle elle appartient, ses pv,si elle à été jouée ce tour 
        // et si on affiche sa range
        int troupe_bleu = 2;
        int troupe_rouge = 1;
        //on suppose que la matrice est de taille inférieure à 1000 * 1000
        int mat_troupes[400][400][4]={{{'\0'}}};

        modify_information(info, "Debut de la partie !");

        do {

            if(nbvilles(villes_bleus) == 0){
                modify_information(info, "Les rouges ont gagne");
            }
            else if(nbvilles(villes_rouges) == 0){
                modify_information(info, "Les bleus ont gagne");
            }

            //Place le curseur et récupère les frappes de clavier
            mvaddch(y, x, CURSEUR);
            ch = getch();


            //Finis un tour si l'utilisateur appuie sur t et si aucune range est affichée
            if(ch == 't' && tour == 'b' && !is_range_display(mat_troupes)){
                tour = 'r';
                reinitialise_tour(mat_troupes);
                nbTroupesBleu = 0;
                modify_information(info, "Nouveau tour");
            }
            else if(ch == 't' && tour == 'r' && !is_range_display(mat_troupes)){
                tour = 'b';
                reinitialise_tour(mat_troupes);
                nbTroupesRouge = 0;  
                modify_information(info, "Nouveau tour");
            }
            else{

            //affiche le tour de l'équipe qui doit jouer        
            affiche_tour(tour);

            //Code apparition des troupes
            if(tour == 'r' && est_pos_ville(x,y,villes_rouges) && ch == '\n' && !is_range_display(mat_troupes)){
                if(nbTroupesRouge < 4){
                    mat_troupes[y][x - 1][0] = troupe_rouge;
                    mat_troupes[y][x - 1][1] = 2;
                    attron(COLOR_PAIR(RED_PAIR));
                    mvaddch(y, x - 1, TROUPE_R);
                    attroff(COLOR_PAIR(RED_PAIR));
                    nbTroupesRouge++;
                    modify_information(info, "Troupe cree");
                }
                else{
                    modify_information(info, "On peut crer au maximum 4 troupes par tour !");
                }
            }
            else if(tour == 'b' && est_pos_ville(x,y,villes_bleus) && ch == '\n' && !is_range_display(mat_troupes)){
                if(nbTroupesBleu < 4){
                    mat_troupes[y][x - 1][0] = troupe_bleu;
                    mat_troupes[y][x - 1][1] = 2;
                    attron(COLOR_PAIR(BLUE_PAIR));
                    mvaddch(y, x - 1, TROUPE_B);
                    attroff(COLOR_PAIR(BLUE_PAIR));
                    nbTroupesBleu++;
                    modify_information(info, "Troupe cree");
                }
                else{
                    modify_information(info, "On peut crer au maximum 4 troupes par tour !");
                }
            }

            //Code de déplacement troupe
            int xtroupe_dep_en_cours = xtroupe_dep(mat_troupes);
            int ytroupe_dep_en_cours = ytroupe_dep(mat_troupes);
            if(tour == 'r' && ch == '\n' && is_move_zone(x, y, xtroupe_dep_en_cours, ytroupe_dep_en_cours, mat_troupes, troupe_rouge) && !est_pos_ville(x,y,villes_rouges) && !est_pos_troupe(x, y, mat_troupes, troupe_rouge)){
                int pv = mat_troupes[x][y][1];
                int is_played = 1;
                //on actualise dans la matrice la position des troupes
                mat_troupes[y][x][0] = troupe_rouge;
                mat_troupes[y][x][1] = pv;
                mat_troupes[y][x][2] = is_played;
                mat_troupes[y][x][3] = 0;
                mat_troupes[ytroupe_dep_en_cours][xtroupe_dep_en_cours][0] = 0;
                mat_troupes[ytroupe_dep_en_cours][xtroupe_dep_en_cours][1] = 0;
                mat_troupes[ytroupe_dep_en_cours][xtroupe_dep_en_cours][2] = 0;
                mat_troupes[ytroupe_dep_en_cours][xtroupe_dep_en_cours][3] = 0;
                erase_move_zone(xtroupe_dep_en_cours, ytroupe_dep_en_cours, mat_troupes, villes_bleus, villes_rouges);
                attron(COLOR_PAIR(GRASS_PAIR));
                mvaddch(ytroupe_dep_en_cours, xtroupe_dep_en_cours, GRASS);
                attroff(COLOR_PAIR(GRASS_PAIR));
                troupe_sous_curseur = 'r';
                modify_information(info, "Troupe deplacee");
                erase_information(info,COLS);
                display_information(info,COLS);
                if(est_pos_troupe(x,y,mat_troupes,troupe_bleu)){
                    mat_troupes[y][x][0] = 0;
                    mat_troupes[y][x][1] = 0;
                    mat_troupes[y][x][2] = 0;
                    mat_troupes[y][x][3] = 0; 
                    modify_information(info, "Troupe tue par les rouges");
                    erase_information(info,COLS);
                    display_information(info,COLS);
                }
                else if(est_pos_ville(x, y, villes_bleus)){
                    modify_information(info, "Ville prise par les rouges");
                    erase_information(info,COLS);
                    display_information(info,COLS);
                    lose_city(x, y, villes_bleus);
                    win_city(x,y, villes_rouges);
                    ville_sous_curseur = 'r';
                    nbTroupesRouge = 4;
                    attron(COLOR_PAIR(RED_PAIR));
                    mvaddch(y, x, RED_CITY);
                    attroff(COLOR_PAIR(RED_PAIR));
                    mat_troupes[y][x][0] = 0;
                    mat_troupes[y][x][1] = 0;
                    mat_troupes[y][x][2] = 0;
                    mat_troupes[y][x][3] = 0; 
                }
            }


            if(tour == 'b' && ch == '\n' && is_move_zone(x, y, xtroupe_dep_en_cours, ytroupe_dep_en_cours, mat_troupes, troupe_bleu) && !est_pos_ville(x,y,villes_bleus) && !est_pos_troupe(x, y, mat_troupes, troupe_bleu)){
                int pv = mat_troupes[x][y][1];
                int is_played = 1;
                //on actualise dans la matrice la position des troupes
                mat_troupes[y][x][0] = troupe_bleu;
                mat_troupes[y][x][1] = pv;
                mat_troupes[y][x][2] = is_played;
                mat_troupes[y][x][3] = 0;
                mat_troupes[ytroupe_dep_en_cours][xtroupe_dep_en_cours][0] = 0;
                mat_troupes[ytroupe_dep_en_cours][xtroupe_dep_en_cours][1] = 0;
                mat_troupes[ytroupe_dep_en_cours][xtroupe_dep_en_cours][2] = 0;
                mat_troupes[ytroupe_dep_en_cours][xtroupe_dep_en_cours][3] = 0;
                erase_move_zone(xtroupe_dep_en_cours, ytroupe_dep_en_cours, mat_troupes, villes_rouges, villes_bleus);
                attron(COLOR_PAIR(GRASS_PAIR));
                mvaddch(ytroupe_dep_en_cours, xtroupe_dep_en_cours, GRASS);
                attroff(COLOR_PAIR(GRASS_PAIR));
                troupe_sous_curseur = 'b';
                modify_information(info, "Troupe deplacee");
                erase_information(info,COLS);
                display_information(info,COLS);
                if(est_pos_troupe(x,y,mat_troupes,troupe_rouge)){
                    mat_troupes[y][x][0] = 0;
                    mat_troupes[y][x][1] = 0;
                    mat_troupes[y][x][2] = 0;
                    mat_troupes[y][x][3] = 0; 
                    modify_information(info, "Troupe tue par les bleus");
                    erase_information(info,COLS);
                    display_information(info,COLS);
                }
                else if(est_pos_ville(x, y, villes_rouges)){
                    modify_information(info, "Ville prise par les bleus");
                    erase_information(info,COLS);
                    display_information(info,COLS);
                    lose_city(x, y, villes_rouges);
                    win_city(x,y, villes_bleus);
                    ville_sous_curseur = 'b';
                    nbTroupesBleu = 4;
                    attron(COLOR_PAIR(BLUE_PAIR));
                    mvaddch(y, x, BLUE_CITY);
                    attroff(COLOR_PAIR(BLUE_PAIR));
                    mat_troupes[y][x][0] = 0;
                    mat_troupes[y][x][1] = 0;
                    mat_troupes[y][x][2] = 0;
                    mat_troupes[y][x][3] = 0; 
                }
            }

    //Affiche / Retire la range de déplacement de la troupe
            if(tour == 'b' && ch == '\n' && mat_troupes[y][x][3] == 0 && mat_troupes[y][x][2] == 0 && est_pos_troupe(x, y, mat_troupes, troupe_bleu) && !is_range_display(mat_troupes)){
                display_move_zone(x, y, mat_troupes, villes_rouges, villes_bleus);
                mat_troupes[y][x][3] = 1;
            }
            else if(mat_troupes[y][x][3] == 1 && ch == '\n' && tour == 'b' && est_pos_troupe(x, y, mat_troupes, troupe_bleu)){
                erase_move_zone(x, y,mat_troupes, villes_rouges, villes_bleus);
                mat_troupes[y][x][3] = 0;
            }
            if (tour == 'r' && ch == '\n' && mat_troupes[y][x][3] == 0 && mat_troupes[y][x][2] == 0 && est_pos_troupe(x, y, mat_troupes, troupe_rouge) && !is_range_display(mat_troupes)){
                display_move_zone(x, y, mat_troupes, villes_rouges, villes_bleus);
                mat_troupes[y][x][3] = 1;
            }
            else if(mat_troupes[y][x][3] == 1 && ch == '\n' && tour == 'r' && est_pos_troupe(x, y, mat_troupes, troupe_rouge)){
                erase_move_zone(x, y, mat_troupes, villes_rouges, villes_bleus);
                mat_troupes[y][x][3] = 0;
            }

            //affiche les informations des action réalisées par le joueur
            erase_information(info, COLS);
            display_information(info, COLS);

            //Code du curseur
            switch (ch) {
            case KEY_UP:
            case 'z':
            case 'Z':
                if (y > 0) {
                    if(ville_sous_curseur == 'r' && troupe_sous_curseur == 'r'){
                        mem_color = RED_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = RED_CITY;
                        ville_sous_curseur = '\0';
                        troupe_sous_curseur = '\0';
                    }
                    else if(troupe_sous_curseur == 'r'){
                        mem_color = RED_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = TROUPE_R;
                        troupe_sous_curseur = '\0';
                    }
                    else if(ville_sous_curseur == 'b' && troupe_sous_curseur == 'b'){
                        mem_color = BLUE_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = BLUE_CITY;
                        ville_sous_curseur = '\0';
                        troupe_sous_curseur = '\0';
                    }
                    else if(troupe_sous_curseur == 'b'){
                        mem_color = BLUE_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = TROUPE_B;
                        troupe_sous_curseur = '\0';
                    }
                    else if((mvinch(y - 1, x) & A_CHARTEXT) == WATER){
                        fut_color = WATER_PAIR;
                    }
                    else if((mvinch(y - 1, x) & A_CHARTEXT) == GRASS){
                        fut_color = GRASS_PAIR;
                    }
                    else if((mvinch(y - 1, x) & A_CHARTEXT) == BLUE_CITY){
                        fut_color = BLUE_PAIR ;
                    }
                    else if((mvinch(y - 1, x) & A_CHARTEXT) == RED_CITY){
                        fut_color = RED_PAIR ;
                    }
                    else if((mvinch(y - 1, x) & A_CHARTEXT) == TROUPE_R){
                        fut_color = RED_PAIR ;
                    }
                    else if((mvinch(y - 1, x) & A_CHARTEXT) == TROUPE_B){
                        fut_color = BLUE_PAIR ;
                    }
                    else if((mvinch(y - 1, x) & A_CHARTEXT) == MOVE_ZONE){
                        fut_color = MAGENTA_PAIR ;
                    }

                    attron(COLOR_PAIR(mem_color));
                    mvaddch(y, x, mem_char);
                    attroff(COLOR_PAIR(mem_color));
                    y = y - 1;
                    mem_char = mvinch(y, x) & A_CHARTEXT;
                    mem_color = mvinch(y, x) & A_COLOR;
                    mem_color = fut_color;
                    
                }
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
            if (y < LINES - 1){
                    if(ville_sous_curseur == 'r' && troupe_sous_curseur == 'r'){
                        mem_color = RED_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = RED_CITY;
                        ville_sous_curseur = '\0';
                        troupe_sous_curseur = '\0';
                    }
                    else if(troupe_sous_curseur == 'r'){
                        mem_color = RED_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = TROUPE_R;
                        troupe_sous_curseur = '\0';
                    }
                    else if(ville_sous_curseur == 'b' && troupe_sous_curseur == 'b'){
                        mem_color = BLUE_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = BLUE_CITY;
                        ville_sous_curseur = '\0';
                        troupe_sous_curseur = '\0';
                    }
                    else if(troupe_sous_curseur == 'b'){
                        mem_color = BLUE_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = TROUPE_B;
                        troupe_sous_curseur = '\0';
                    }
                    else if((mvinch(y +  1, x) & A_CHARTEXT) == WATER){
                        fut_color = WATER_PAIR;
                    }
                    else if((mvinch(y + 1, x) & A_CHARTEXT) == ' '){
                        fut_color = GRASS_PAIR;
                    }
                    else if((mvinch(y + 1, x) & A_CHARTEXT) == BLUE_CITY){
                        fut_color = BLUE_PAIR ;
                    }
                    else if((mvinch(y + 1, x) & A_CHARTEXT) == RED_CITY){
                        fut_color = RED_PAIR ;
                    }
                    else if((mvinch(y + 1, x) & A_CHARTEXT) == TROUPE_R){
                        fut_color = RED_PAIR ;
                    }
                    else if((mvinch(y + 1, x) & A_CHARTEXT) == TROUPE_B){
                        fut_color = BLUE_PAIR ;
                    }
                    else if((mvinch(y + 1, x) & A_CHARTEXT) == MOVE_ZONE){
                        fut_color = MAGENTA_PAIR ;
                    }


                    attron(COLOR_PAIR(mem_color));
                    mvaddch(y, x, mem_char);
                    attroff(COLOR_PAIR(mem_color));
                    y++;
                    mem_char = mvinch(y, x) & A_CHARTEXT;
                    mem_color = mvinch(y, x) & A_COLOR;
                    mem_color = fut_color;
                }
                break;
            case KEY_LEFT:
            case 'q':
            case 'Q':
            if (x > 0){
                    if(ville_sous_curseur == 'r' && troupe_sous_curseur == 'r'){
                        mem_color = RED_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = RED_CITY;
                        ville_sous_curseur = '\0';
                        troupe_sous_curseur = '\0';
                    }
                    else if(troupe_sous_curseur == 'r'){
                        mem_color = RED_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = TROUPE_R;
                        troupe_sous_curseur = '\0';
                    }
                    else if(ville_sous_curseur == 'b' && troupe_sous_curseur == 'b'){
                        mem_color = BLUE_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = BLUE_CITY;
                        ville_sous_curseur = '\0';
                        troupe_sous_curseur = '\0';
                    }
                    else if(troupe_sous_curseur == 'b'){
                        mem_color = BLUE_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = TROUPE_B;
                        troupe_sous_curseur = '\0';
                    }
                    else if((mvinch(y, x  - 1) & A_CHARTEXT) == WATER){
                        fut_color = WATER_PAIR;
                    }
                    else if((mvinch(y, x - 1) & A_CHARTEXT) == ' '){
                        fut_color = GRASS_PAIR;
                    }
                    else if((mvinch(y, x - 1) & A_CHARTEXT) == BLUE_CITY){
                        fut_color = BLUE_PAIR ;
                    }
                    else if((mvinch(y, x - 1) & A_CHARTEXT) == RED_CITY){
                        fut_color = RED_PAIR ;
                    }
                    else if((mvinch(y, x - 1) & A_CHARTEXT) == TROUPE_R){
                        fut_color = RED_PAIR ;
                    }
                    else if((mvinch(y, x - 1) & A_CHARTEXT) == TROUPE_B){
                        fut_color = BLUE_PAIR ;
                    }
                    else if((mvinch(y, x - 1) & A_CHARTEXT) == MOVE_ZONE){
                        fut_color = MAGENTA_PAIR ;
                    }


                    attron(COLOR_PAIR(mem_color));
                    mvaddch(y, x, mem_char);
                    attroff(COLOR_PAIR(mem_color));
                    x--;
                    mem_char = mvinch(y, x) & A_CHARTEXT;
                    mem_color = mvinch(y, x) & A_COLOR;
                    mem_color = fut_color;
                }
                break;
            case KEY_RIGHT:
            case 'd':
            case 'D':
            if (x < COLS - 1){
                    if(ville_sous_curseur == 'r' && troupe_sous_curseur == 'r'){
                        mem_color = RED_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = RED_CITY;
                        ville_sous_curseur = '\0';
                        troupe_sous_curseur = '\0';
                    }
                    else if(troupe_sous_curseur == 'r'){
                        mem_color = RED_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = TROUPE_R;
                        troupe_sous_curseur = '\0';
                    }
                    else if(ville_sous_curseur == 'b' && troupe_sous_curseur == 'b'){
                        mem_color = BLUE_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = BLUE_CITY;
                        ville_sous_curseur = '\0';
                        troupe_sous_curseur = '\0';
                    }
                    else if(troupe_sous_curseur == 'b'){
                        mem_color = BLUE_PAIR;
                        fut_color = GRASS_PAIR;
                        mem_char = TROUPE_B;
                        troupe_sous_curseur = '\0';
                    }
                    else if((mvinch(y, x + 1) & A_CHARTEXT) == WATER){
                        fut_color = WATER_PAIR;
                    }
                    else if((mvinch(y, x + 1) & A_CHARTEXT) == ' '){
                        fut_color = GRASS_PAIR;
                    }
                    else if((mvinch(y, x + 1) & A_CHARTEXT) == BLUE_CITY){
                        fut_color = BLUE_PAIR ;
                    }
                    else if((mvinch(y, x + 1) & A_CHARTEXT) == RED_CITY){
                        fut_color = RED_PAIR ;
                    }
                    else if((mvinch(y, x + 1) & A_CHARTEXT) == TROUPE_R){
                        fut_color = RED_PAIR ;
                    }
                    else if((mvinch(y, x + 1) & A_CHARTEXT) == TROUPE_B){
                        fut_color = BLUE_PAIR ;
                    }
                    else if((mvinch(y, x + 1) & A_CHARTEXT) == MOVE_ZONE){
                        fut_color = MAGENTA_PAIR ;
                    }


                    attron(COLOR_PAIR(mem_color));
                    mvaddch(y, x, mem_char);
                    attroff(COLOR_PAIR(mem_color));
                    x++;
                    mem_char = mvinch(y, x) & A_CHARTEXT;
                    mem_color = mvinch(y, x) & A_COLOR;
                    mem_color = fut_color;
                }
                break;
            }
        }
    }
        while (nbvilles(villes_bleus) != 0 && nbvilles(villes_rouges) != 0);
        //endwin();
        clear();
        printf("\033[H\033[J");
        accueil();
        exit(0);
    }

/*int move_okay(int y, int x)
{
    int testch;
     return true if the space is okay to move into 
    testch = mvinch(y, x);
    return ((((testch & A_CHARTEXT) == GRASS)) || ((testch & A_CHARTEXT) == VILLE));
}
*/

int where(int y, int x)
{
    int testch;

    /* regarde ou je suis haha c'est drole ca fait pas 1j que je travaille dessus */

    testch = mvinch(y, x);
    return ((testch & A_COLOR) == COLOR_PAIR(PLAYER_PAIR));
}

int check(int y, int x)
{
    int testch;

    /* regarde ou je suis haha c'est drole ca fait pas 1j que je travaille dessus */

    testch = mvinch(y, x);
    return (testch & A_COLOR);
}

void draw_map(){
    int y; 

    /* draw the quest map */

    /* background */

    attron(COLOR_PAIR(WATER_PAIR));
    for (y = 0; y < LINES; y++) {
        mvhline(y, 0, WATER, COLS);
    }
    attroff(COLOR_PAIR(WATER_PAIR));

    /* debut de l'enfer */

    /* Ile en haut a gauche */

    attron(COLOR_PAIR(GRASS_PAIR));
    for (y = LINES / 4; y < (3 * LINES) / 4; y++) {
        mvhline(y, 1, GRASS, COLS / 3);
    }    
    attroff(COLOR_PAIR(GRASS_PAIR));

    /* Ile en haut a droite */

    attron(COLOR_PAIR(GRASS_PAIR));
    for (y = LINES / 4; y < (3 * LINES) / 4; y++) {
        mvhline(y, (COLS * 2 )/ 3, GRASS, (COLS / 3) - 1);
    }    
    attroff(COLOR_PAIR(GRASS_PAIR));

    /* Pont entre les deux */

    attron(COLOR_PAIR(GRASS_PAIR));
    for (y = (LINES / 2) - 2; y < (LINES / 2) + 2; y++) {
        mvhline(y, 1, GRASS, COLS - 2);
    }    
    attroff(COLOR_PAIR(GRASS_PAIR));

    int ordV1R = YV1R, absV1R = XV1R;

    attron(COLOR_PAIR(RED_PAIR));
    mvaddch(ordV1R, absV1R, RED_CITY); //Première ville rouge
    attroff(COLOR_PAIR(RED_PAIR));

    int ordV2R = YV2R, absV2R = XV2R;

    attron(COLOR_PAIR(RED_PAIR));
    mvaddch(ordV2R, absV2R, RED_CITY);  //Deuxième ville rouge
    attroff(COLOR_PAIR(RED_PAIR));

    int ordV1B = YV1B, absV1B = XV1B;

    attron(COLOR_PAIR(BLUE_PAIR));
    mvaddch(ordV1B, absV1B, BLUE_CITY);  //Première ville bleu
    attroff(COLOR_PAIR(BLUE_PAIR));

    int ordV2B = YV2B, absV2B = XV2B;

    attron(COLOR_PAIR(BLUE_PAIR));
    mvaddch(ordV2B, absV2B, BLUE_CITY); //Deuxième ville bleu
    attroff(COLOR_PAIR(BLUE_PAIR));

}

bool est_gagne(){
    return true;
}

int main(){
    accueil();
}
