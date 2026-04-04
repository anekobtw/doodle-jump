#include "player.h"

void erase_player_at(WINDOW* win, int x, int y) {
  mvwaddch(win, y - 1, x, ' ');
  mvwaddch(win, y, x, ' ');
  mvwaddch(win, y + 1, x, ' ');
}

void draw_player_at(WINDOW* win, int x, int y, char symbol) {
  mvwaddch(win, y - 1, x, symbol);
  mvwaddch(win, y, x, symbol);
  mvwaddch(win, y + 1, x, symbol);
}

void clamp_player(Player* p) {
  if (p->x < 3) p->x = 3;
  if (p->x > WIN_X - 2) p->x = WIN_X - 2;
  if (p->y < 2) p->y = 2;
  if (p->y > WIN_Y - 3) p->y = WIN_Y - 3;
}

void draw_player(WINDOW* win, Player* p) {
  clamp_player(p);
  mvwaddch(win, p->y - 1, p->x, p->symbol);
  mvwaddch(win, p->y, p->x, p->symbol);
  mvwaddch(win, p->y + 1, p->x, p->symbol);
  wrefresh(win);
}

void move_player(WINDOW* win, Player* p, int dx, int dy) {
  float cur_x = p->x, cur_y = p->y;
  int last_x = p->x, last_y = p->y;

  for (int i = 0; i < 5; i++) {
    cur_x += (float)dx / 5;
    cur_y -= (float)dy / 5;
    int new_x = (int)cur_x;
    int new_y = (int)cur_y;

    if (new_x != last_x || new_y != last_y) {
      erase_player_at(win, last_x, last_y);
      draw_player_at(win, new_x, new_y, p->symbol);
      box(win, 0, 0);
      wrefresh(win);
      napms(30);
      last_x = new_x;
      last_y = new_y;
    }
  }

  p->x += dx;
  p->y -= dy;
  clamp_player(p);
  erase_player_at(win, last_x, last_y);
  draw_player(win, p);
  wrefresh(win);
}
