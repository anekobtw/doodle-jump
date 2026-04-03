#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "platform.h"
#include "player.h"

#define WIN_X 60
#define WIN_Y 30

#define PLATFORM_COUNT_MAX 10
#define GRAVITY_TICKS 8*PLATFORM_COUNT_MAX
#define JUMP_HEIGHT 10

int main() {
  srand(time(NULL));
  initscr();
  noecho();
  curs_set(0);

  // Window
  WINDOW* win = newwin(WIN_Y, WIN_X, 0, 0);
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
  Player player = {WIN_X / 2, WIN_Y / 4, PLAYER_CHAR};
  draw_player(win, &player);

  // Game loop
  bool exit = false;
  nodelay(win, TRUE);
  cbreak();
  Platform platforms[PLATFORM_COUNT_MAX];
  int curr_plat_count = 0;
  int gravity_tick = 0;

  while (!exit) {
    switch (wgetch(win)) {
      case KEY_UP:
        move_player(win, &player, 0, 1);
        break;
      case KEY_RIGHT:
        move_player(win, &player, 1, 0);
        break;
      case KEY_LEFT:
        move_player(win, &player, -1, 0);
        break;
      case 27:
        exit = true;
        break;  // escape
    }

    if (curr_plat_count < PLATFORM_COUNT_MAX) {
      platforms[curr_plat_count] = create_random_platform();
      draw_platform(win, &platforms[curr_plat_count]);
      curr_plat_count++;
    }

    for (int i = 0; i < curr_plat_count; i++) {
      if (player.y + 2 == platforms[i].y && player.x >= platforms[i].x + 1 &&
          player.x <= platforms[i].x + platforms[i].length) {
        // The player is on some platform
        move_player(win, &player, 0, JUMP_HEIGHT);
        if (player.y <= WIN_Y / 2) {
          for (int i = 0; i < curr_plat_count; i++) {
            move_platform(win, &platforms[i], 0, -1);
          }
        }
        gravity_tick = 0;
        break;
      } else {
        gravity_tick++;
        if (gravity_tick >= GRAVITY_TICKS) {
          move_player(win, &player, 0, -1);
          gravity_tick = 0;
        }
      }
    }

    napms(20);
  }

  endwin();
  return 0;
}
