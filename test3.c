#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define GRASS     ' '
#define WATER     '~'
#define MOUNTAIN  '^'
#define TROUPE      '&'
#define CURSEUR    'O'
#define BLUE_CITY     'X'
#define RED_CITY     'x'

#define GRASS_PAIR     1
#define WATER_PAIR     2
#define MOUNTAIN_PAIR  3
#define PLAYER_PAIR    4
#define RED_PAIR     5
#define BLUE_PAIR   6

/* int is_move_okay(int y, int x); */ 
void draw_map(void);
int where(int y, int x);

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



int jeux()
{
    int y, x;
    int ch;
    char tour = 'b';

    /* initialize curses */

    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    /* initialize colors */
    start_color();
    init_pair(GRASS_PAIR, COLOR_WHITE, COLOR_GREEN);
    init_pair(WATER_PAIR, COLOR_CYAN, COLOR_BLUE);
    init_pair(MOUNTAIN_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(PLAYER_PAIR, COLOR_RED, COLOR_MAGENTA);
    init_pair(RED_PAIR, COLOR_WHITE, COLOR_RED);
    init_pair(BLUE_PAIR, COLOR_WHITE, COLOR_BLUE);

    clear();

    /* initialise la map */

    draw_map();

    
    y = LINES / 2;
    x = COLS / 2;


    /* On crée une mémoire pour rechanger la couleur et le caractère en dessous */

    char mem_char = GRASS;
    int mem_color = GRASS_PAIR;
    int fut_color = GRASS_PAIR;


    do {
        //Place le curseur et récupère les frappes de clavier

        mvaddch(y, x, CURSEUR);
        ch = getch();

        //Finis un tour si l'utilisateur appuie sur t

        if(ch == 't' && tour == 'b'){
            tour = 'r';
        }
        else if(ch == 't' && tour == 'r'){
            tour = 'b';
        }
        else{

        //affiche le tour de l'équipe qui doit jouer        

        affiche_tour(tour);

            //Code du curseur
        switch (ch) {
        case KEY_UP:
        case 'z':
        case 'Z':
            if (y > 0) {
                if((mvinch(y - 1, x) & A_CHARTEXT) == WATER){
                    fut_color = WATER_PAIR;
                }
                else if((mvinch(y - 1, x) & A_CHARTEXT) == ' '){
                    fut_color = GRASS_PAIR;
                }
                else if((mvinch(y - 1, x) & A_CHARTEXT) == BLUE_CITY){
                    fut_color = BLUE_PAIR ;
                }
                else if((mvinch(y - 1, x) & A_CHARTEXT) == RED_CITY){
                    fut_color = RED_PAIR ;
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
                if((mvinch(y +  1, x) & A_CHARTEXT) == WATER){
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
                if((mvinch(y, x  - 1) & A_CHARTEXT) == WATER){
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
                if((mvinch(y, x + 1) & A_CHARTEXT) == WATER){
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
    while ((ch != 'p') && (ch != 'P'));

    endwin();

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

    int ordV1R = 18, absV1R = 49;

    attron(COLOR_PAIR(RED_PAIR));
    mvaddch(ordV1R, absV1R, RED_CITY); //Première ville rouge
    attroff(COLOR_PAIR(RED_PAIR));

    int ordV2R = 30, absV2R = 10;

    attron(COLOR_PAIR(RED_PAIR));
    mvaddch(ordV2R, absV2R, RED_CITY);  //Deuxième ville rouge
    attroff(COLOR_PAIR(RED_PAIR));

    int ordV1B = 30, absV1B = 200;

    attron(COLOR_PAIR(BLUE_PAIR));
    mvaddch(ordV1B, absV1B, BLUE_CITY);  //Première ville bleu
    attroff(COLOR_PAIR(BLUE_PAIR));

    int ordV2B = 15, absV2B = 170;

    attron(COLOR_PAIR(BLUE_PAIR));
    mvaddch(ordV2B, absV2B, BLUE_CITY); //Deuxième ville bleu
    attroff(COLOR_PAIR(BLUE_PAIR));

}

bool est_gagne(){
    return true;
}

int main(){
    while(est_gagne()){
            jeux();
    }
}
