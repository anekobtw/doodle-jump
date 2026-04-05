#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "platform.h"
#include "player.h"
#include "stats.h"

#define WIN_X 60
#define WIN_Y 30

#define FRAME_DELAY_MS 15     // more = game slower, less = game faster
#define GRAVITY_TICKS_MAX 10  // more = gravity weaker, less = gravity stronger

#define PLATFORMS_COUNT_MAX 15
#define JUMP_STEPS 6
#define PLATFORM_SCROLL_STEPS 2

static void draw_hud(int score, bool game_over) {
  const int hud_x = WIN_X + 2;
  char score_text[32];
  char best_score_text[32];
  const char* lines[5];
  snprintf(score_text, sizeof(score_text), "Score: %d", score);
  snprintf(best_score_text, sizeof(best_score_text), "Best score: %d",
           load_bestscore());

  if (game_over) {
    lines[0] = "GAME OVER!";
    lines[1] = "Press R to restart";
    lines[2] = "Press ESC to exit";
    lines[3] = " ";
    lines[4] = " ";
  } else {
    lines[0] = "Doodle Jump (c) anekobtw, 2026";
    lines[1] = score_text;
    lines[2] = best_score_text;
    lines[3] = "Arrow keys: Move left/right";
    lines[4] = "ESC: Quit the game";
  }

  int n = sizeof(lines) / sizeof(lines[0]);

  if (LINES < n + 1 || COLS <= hud_x + 10) return;

  for (int i = 0; i < n; i++) {
    move(i + 1, hud_x);
    clrtoeol();
    mvprintw(i + 1, hud_x, "%s", lines[i]);
  }
}

void game_init(Player* p, Platform platforms[],
               int* curr_plat_count, int* gravity_tick, int* score) {
  *p = (Player){WIN_X / 2, WIN_Y / 4, PLAYER_CHAR};
  *curr_plat_count = 0;
  *gravity_tick = 0;
  *score = 0;

  for (size_t i = 0; i < PLATFORMS_COUNT_MAX; i++) {
    platforms[i] = create_random_platform(false);
    (*curr_plat_count)++;
  }
}

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
  Player player;
  Platform platforms[PLATFORMS_COUNT_MAX];
  int curr_plat_count, gravity_tick, score;

  game_init(&player, platforms, &curr_plat_count, &gravity_tick, &score);

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
      for (size_t i = 0; i < JUMP_STEPS; i++) {
        // if a player on platform, then jump
        move_player(win, &player, 0, 1);

        // move platforms down and replace them with new ones if needed
        if (player.y <= WIN_Y / 2 && i < PLATFORM_SCROLL_STEPS) {
          score++;

          for (size_t j = 0; j < curr_plat_count; j++) {
            move_platform(win, &platforms[j], 0, -1);
            if (platforms[j].y == WIN_Y - 2) {
              platforms[j] = create_random_platform(true);
            }
          }
        }
      }
    } else {
      if (gravity_tick >= GRAVITY_TICKS_MAX) {
        move_player(win, &player, 0, -1);
        gravity_tick = 0;
      }
    }

    // Update hud
    if (player.y == WIN_Y - 3) {
      draw_hud(score, true);
      int ch = wgetch(win);
      if (ch == 27) {
        break;
      } else if (ch == 82 || ch == 114) {
        game_init(&player, platforms, &curr_plat_count, &gravity_tick,
                  &score);
      }
    } else {
      draw_hud(score, false);
    }

    werase(win);
    box(win, 0, 0);
    draw_player(win, &player);
    redraw_platforms(win, platforms, curr_plat_count);
    refresh();
    wrefresh(win);
    napms(FRAME_DELAY_MS);
    save(score);
  }

  endwin();
  return 0;
}
