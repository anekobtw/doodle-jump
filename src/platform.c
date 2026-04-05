#include "platform.h"

void draw_platform(WINDOW* win, Platform* p) {
  for (int i = 1; i <= p->length; i++) {
    if (p->x + i > 1 && p->x + i < WIN_X - 1) {
      mvwaddch(win, p->y, p->x + i, p->symbol);
    }
  }
};

void redraw_platforms(WINDOW* win, Platform platforms[], int count) {
  for (int i = 0; i < count; i++) {
    draw_platform(win, &platforms[i]);
  }
}

void erase_platform(WINDOW* win, Platform* p) {
  for (int i = 1; i <= p->length; i++) {
    if (p->x + i > 1 && p->x + i < WIN_X - 1) {
      mvwaddch(win, p->y, p->x + i, ' ');
    }
  }
}

void move_platform(WINDOW* win, Platform* p, int dx, int dy) {
  (void)win;

  p->x += dx;
  p->y -= dy;

  if (p->x < 1) p->x = 1;
  if (p->x > WIN_X - 1 - p->length) p->x = WIN_X - 1 - p->length;
  if (p->y < 1) p->y = 1;
  if (p->y > WIN_Y - 2) p->y = WIN_Y - 2;
}

Platform create_random_platform(bool on_top) {
  Platform p;
  if (on_top)
    p.y = 1;
  else
    p.y = 1 + rand() % (WIN_Y - 2);
  p.x = 1 + rand() % (WIN_X - 2);
  p.length = 3 + rand() % 3;
  p.symbol = PLATFORM_CHAR;
  return p;
}
