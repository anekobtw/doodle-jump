#ifndef PLATFORM_H
#define PLATFORM_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WIN_X 60
#define WIN_Y 30

#define PLATFORM_CHAR '='

typedef struct {
  int x, y;
  int length;
  char symbol;
} Platform;

void draw_platform(WINDOW* win, Platform* p);
void redraw_platforms(WINDOW* win, Platform platforms[], int count);
void erase_platform(WINDOW* win, Platform* p);
void move_platform(WINDOW* win, Platform* p, int dx, int dy);
Platform create_random_platform(bool on_top);

#endif