#include <ncurses.h>
#include <stdbool.h>

#define WIN_X 90
#define WIN_Y 30
#define PLAYER_CHAR '0'
#define PLATFORM_CHAR '#'

typedef struct {
  int x, y;
  char symbol;
} Player;

typedef struct {
  int x, y, width;
  char symbol;
} Platform;

void clamp_player(Player *p) {
    if (p->x < 1)       p->x = 1;
    if (p->x > WIN_X-2) p->x = WIN_X-2;
    if (p->y < 2)       p->y = 2;
    if (p->y > WIN_Y-3) p->y = WIN_Y-3;
}

void draw_player(WINDOW *win, Player *p) {
  clamp_player(p);
  mvwaddch(win, p->y - 1, p->x, p->symbol);
  mvwaddch(win, p->y, p->x, p->symbol);
  mvwaddch(win, p->y + 1, p->x, p->symbol);
  wrefresh(win);
}

void move_player(WINDOW *win, Player *p, int dx, int dy) {
  // erase old position
  mvwaddch(win, p->y-1, p->x, ' ');
  mvwaddch(win, p->y, p->x, ' ');
  mvwaddch(win, p->y+1, p->x, ' ');
  // draw a new one
  p->x += dx;
  p->y -= dy; // y is inverted, so it's -= and not +=
  draw_player(win, p);
  wrefresh(win);
}

void draw_platform(WINDOW *win, Platform *p) {
  for (int i = 1; i <= p->width; i++) {
    if (p->x+i > 1 && p->x+i < WIN_X-1) {
      mvwaddch(win, p->y, p->x+i, p->symbol);
    }
  }
  wrefresh(win);
};

int main() {
  initscr();
  noecho();
  curs_set(0);
  
  // Window
  WINDOW *win = newwin(WIN_Y, WIN_X, 0, 0);
  keypad(win, TRUE);
  if (win == NULL) {
    endwin();
    printf("Window couldn't be drawn because the terminal is too small");
    return 1;
  }

  box(win, 0, 0);
  refresh();
  wrefresh(win);

  // Player
  Player player = {WIN_X/2, WIN_Y/2, PLAYER_CHAR};
  draw_player(win, &player);

  // Game loop
  bool exit = false;
  nodelay(win, TRUE);
  while (!exit) {
    int ch = wgetch(win);
    switch (ch) {
      case KEY_UP: move_player(win, &player, 0, 1); break;
      case KEY_DOWN: move_player(win, &player, 0, -1); break;
      case KEY_RIGHT: move_player(win, &player, 1, 0); break;
      case KEY_LEFT: move_player(win, &player, -1, 0); break;
      case 27: exit = true; break; // escape
    }
  }

  endwin();
  return 0;
}
