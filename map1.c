    /* quest.c */

#include <curses.h>
#include <stdlib.h>

#define GRASS     ' '
#define WATER     '~'
#define MOUNTAIN  '^'
#define PLAYER    '*'
#define VILLE     '&'

#define GRASS_PAIR     1
#define WATER_PAIR     2
#define MOUNTAIN_PAIR  3
#define PLAYER_PAIR    4
#define VILLE_PAIR     5

int is_move_okay(int y, int x);
void draw_map(void);
int where(int y, int x);

int main(void)
{
    int y, x;
    int ch;

    /* initialize curses */

    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    /* initialize colors */
    start_color();
    init_pair(GRASS_PAIR, COLOR_YELLOW, COLOR_GREEN);
    init_pair(WATER_PAIR, COLOR_CYAN, COLOR_BLUE);
    init_pair(MOUNTAIN_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(PLAYER_PAIR, COLOR_RED, COLOR_MAGENTA);
    init_pair(VILLE_PAIR, COLOR_WHITE, COLOR_BLACK);

    clear();

    /* initialize the quest map */

    draw_map();

    /* start player at lower-left */

    y = LINES - 1;
    x = 0;

    do {
                            
        /* by default, you get a blinking cursor - use it to
           indicate player * */

        attron(COLOR_PAIR(PLAYER_PAIR));
        mvaddch(y, x, PLAYER);
        attroff(COLOR_PAIR(PLAYER_PAIR));
        move(y, x);
        refresh();

        ch = getch();

        /* test inputted key and determine direction */

        switch (ch) {
        case KEY_UP:
        case 'w':
        case 'W':
            if ((y > 0) && is_move_okay(y - 1, x)) {
                if (where(y, x)){
                attron(COLOR_PAIR(GRASS_PAIR));
                mvaddch(y, x, GRASS);
                attroff(COLOR_PAIR(GRASS_PAIR));
                y = y - 1;
                } else {
                attron(COLOR_PAIR(VILLE_PAIR));
                mvaddch(y, x, VILLE);
                attroff(COLOR_PAIR(VILLE_PAIR));
                y = y - 1;
                }
            }
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
        if ((y < LINES - 1) && is_move_okay(y + 1, x)) {
            if (where(y, x)){
                attron(COLOR_PAIR(GRASS_PAIR));
                mvaddch(y, x, GRASS);
                attroff(COLOR_PAIR(GRASS_PAIR));
                y = y + 1;
                } else {
                attron(COLOR_PAIR(VILLE_PAIR));
                mvaddch(y, x, VILLE);
                attroff(COLOR_PAIR(VILLE_PAIR));
                y = y + 1;
                }
        }
            break;
        case KEY_LEFT:
        case 'a':
        case 'A':
        if ((x > 0) && is_move_okay(y, x - 1)) {
            if (where(y, x)){
                attron(COLOR_PAIR(GRASS_PAIR));
                mvaddch(y, x, GRASS);
                attroff(COLOR_PAIR(GRASS_PAIR));
                x = x - 1;
                } else {
                attron(COLOR_PAIR(VILLE_PAIR));
                mvaddch(y, x, VILLE);
                attroff(COLOR_PAIR(VILLE_PAIR));
                x = x - 1;
                }
        }
            break;
        case KEY_RIGHT:
        case 'd':
        case 'D':
        if ((x < COLS - 1) && is_move_okay(y, x + 1)) {
            if (where(y, x)){
                attron(COLOR_PAIR(GRASS_PAIR));
                mvaddch(y, x, GRASS);
                attroff(COLOR_PAIR(GRASS_PAIR));
                x = x + 1;
                } else {
                attron(COLOR_PAIR(VILLE_PAIR));
                mvaddch(y, x, VILLE);
                attroff(COLOR_PAIR(VILLE_PAIR));
                x = x + 1;
                }
        }
            break;
        }
    }
    while ((ch != 'q') && (ch != 'Q'));

    endwin();

    exit(0);
}

int is_move_okay(int y, int x)
{
    int testch;

    /* return true if the space is okay to move into */

    testch = mvinch(y, x);
    return ((((testch & A_CHARTEXT) == GRASS)) || ((testch & A_CHARTEXT) == VILLE));
}

int where(int y, int x)
{
    int testch;

    /* regarde ou je suis haha c'est drole ca fait pas 2h que je travaille dessus */

    testch = mvinch(y, x);
    return ((testch & A_CHARTEXT) == GRASS);
}

void draw_map(void){
    int y, x;

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
}