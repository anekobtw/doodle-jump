#ifndef PLAYER_H
#define PLAYER_H

#include <ncurses.h>

#define PLAYER_CHAR '0'

typedef struct {
  int x, y;
  char symbol;
} Player;

void clamp_player(Player* p);
void draw_player(WINDOW* win, Player* p);
void move_player(WINDOW* win, Player* p, int dx, int dy);

#endif
