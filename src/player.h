#ifndef PLAYER_H
#define PLAYER_H

#include <ncurses.h>

#define WIN_X 60
#define WIN_Y 30

#define PLAYER_CHAR '0'

typedef struct {
  int x, y;
  char symbol;
} Player;

void clamp_player(Player* p);
void draw_player(WINDOW* win, Player* p);
void move_player(WINDOW* win, Player* p, int dx, int dy);

#endif
