#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "platform.h"
#include "player.h"

#define WIN_X 60
#define WIN_Y 30

#define PLATFORM_COUNT_MAX 10
#define GRAVITY_TICKS 8 * PLATFORM_COUNT_MAX
#define JUMP_HEIGHT 10

int main() {
  // Window and the box
  initscr();
  WINDOW* win = newwin(WIN_Y, WIN_X, 0, 0);
  if (win == NULL) {
    endwin();
    printf("Window couldn't be drawn because the terminal is too small");
    return 1;
  }

  box(win, 0, 0);
  refresh();
  wrefresh(win);

  // Config
  srand(time(NULL));
  noecho();
  curs_set(0);
  keypad(win, TRUE);
  nodelay(win, TRUE);
  cbreak();

  // Player
  Player player = {WIN_X / 2, WIN_Y / 4, PLAYER_CHAR};
  draw_player(win, &player);

  // Game loop
  bool exit = false;
  Platform platforms[PLATFORM_COUNT_MAX];
  int curr_plat_count = 0;
  int gravity_tick = 0;
  int horizontal_dx = 0;

  while (!exit) {
    bool left_pressed = false;
    bool right_pressed = false;
    int ch;

    while ((ch = wgetch(win)) != ERR) {
      switch (ch) {
        case KEY_UP:
          move_player(win, &player, 0, 2);
          break;
        case KEY_RIGHT:
          right_pressed = true;
          break;
        case KEY_LEFT:
          left_pressed = true;
          break;
        case 27:
          exit = true;
          break;  // escape
      }
    }

    if (right_pressed && !left_pressed)
      horizontal_dx = 2;
    else if (left_pressed && !right_pressed)
      horizontal_dx = -2;
    else
      horizontal_dx = 0;

    move_player(win, &player, horizontal_dx, 0);

    if (curr_plat_count < PLATFORM_COUNT_MAX) {
      platforms[curr_plat_count] = create_random_platform(false);
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
            move_platform(win, &platforms[i], 0, -2);
          }
          if ((rand() % 4) < 3) {
            platforms[curr_plat_count] = create_random_platform(true);
            draw_platform(win, &platforms[curr_plat_count]);
            curr_plat_count++;
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
