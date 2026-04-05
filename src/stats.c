#include "stats.h"

int load_bestscore() {
  FILE* file = fopen(FILENAME, "r");
  if (!file) return 0;

  int best = 0;
  fscanf(file, "%d", &best);
  fclose(file);

  return best;
}

void save(int score) {
  int best = load_bestscore();

  if (score > best) {
    FILE* file = fopen(FILENAME, "w");
    if (!file) return;

    fprintf(file, "%d", score);
    fclose(file);
  }
}
