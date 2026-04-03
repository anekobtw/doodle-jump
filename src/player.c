#include "player.h"

#include "platform.h"

void clamp_player(Player* p) {
  if (p->x < 1) p->x = 1;
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
  mvwaddch(win, p->y - 1, p->x, ' ');
  mvwaddch(win, p->y, p->x, ' ');
  mvwaddch(win, p->y + 1, p->x, ' ');

  p->x += dx;
  p->y -= dy;  // y is inverted, so it's -= and not +=

  draw_player(win, p);
  wrefresh(win);
}
