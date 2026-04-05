#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "platform.h"
#include "player.h"

#define WIN_X 60
#define WIN_Y 30

#define FRAME_DELAY_MS 15  // more = game slower, less = game faster

#define PLATFORMS_COUNT_MAX 10
#define GRAVITY_TICKS_MAX 10
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

  // Some variables
  Platform platforms[PLATFORMS_COUNT_MAX];
  int curr_plat_count = 0;
  int gravity_tick = 0;

  // Create inital platforms before starting the game loop
  for (size_t i = 0; i < PLATFORMS_COUNT_MAX; i++) {
    platforms[curr_plat_count] = create_random_platform(false);
    curr_plat_count++;
  }

  while (true) {
    gravity_tick++;

    // Check for input
    int horizontal_dx = 0;
    int ch;
    while ((ch = wgetch(win)) != ERR) {
      switch (ch) {
        case KEY_RIGHT:
          horizontal_dx = 2;
          break;
        case KEY_LEFT:
          horizontal_dx = -2;
          break;
        case 27:  // escape
          endwin();
          return 0;
          break;
      }
    }

    if (horizontal_dx != 0) move_player(win, &player, horizontal_dx, 0);

    // Check if a player is on a platform
    // clang-format off
    bool on_platform = false;
    for (size_t i = 0; i < curr_plat_count; i++) {
      if (player.y + 2 == platforms[i].y &&
          player.x >= platforms[i].x + 1 &&
          player.x <= platforms[i].x + platforms[i].length) {
        on_platform = true;
        break;
      }
    }
    // clang-format on

    if (on_platform) {
      // if a player on platform, then jump
      move_player(win, &player, 0, JUMP_HEIGHT);

      // move platforms down and replace them with new ones if needed
      if (player.y <= WIN_Y / 2) {
        for (size_t i = 0; i < curr_plat_count; i++) {
          move_platform(win, &platforms[i], 0, -2);
          if (platforms[i].y == WIN_Y - 2) {
            platforms[i] = create_random_platform(true);
          }
        }
      }
    } else {
      if (gravity_tick >= GRAVITY_TICKS_MAX) {
        move_player(win, &player, 0, -1);
        gravity_tick = 0;
      }
    }

    werase(win);
    box(win, 0, 0);
    draw_player(win, &player);
    redraw_platforms(win, platforms, curr_plat_count);
    wrefresh(win);
    napms(FRAME_DELAY_MS);
  }

  endwin();
  return 0;
}
