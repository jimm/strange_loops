// Defines positions and sizes of windows.
#include <ncurses.h>
#include "geometry.h"

#define MAX_PROMPT_WINDOW_WIDTH 40

inline int INFO_WIDTH() { return COLS * 2 / 3; }

rect geom_message_rect() {
  rect r;

  r.row = LINES - 1;
  r.col = 0;
  r.height = 1;
  r.width = COLS;
  return r;
}

rect geom_track_editor_rect() {
  rect r;

  r.row = 3;
  r.col = 3;
  r.height = LINES - 6;
  r.width = COLS - 6;
  return r;
}

rect geom_info_rect() {
  rect r;

  r.row = 0;
  r.col = INFO_WIDTH();
  r.height = ((LINES - 1) * 2 / 3);
  r.width = COLS - INFO_WIDTH();
  return r;
}

rect geom_help_rect() {
  rect r;

  r.row = 3;
  r.col = 3;
  r.height = LINES - 6;
  r.width = COLS - 6;
  return r;
}

rect geom_prompt_rect() {
  rect r;

  r.height = 3;
  r.width = COLS / 2;
  if (r.width > MAX_PROMPT_WINDOW_WIDTH)
    r.width = MAX_PROMPT_WINDOW_WIDTH;
  r.row = LINES / 3;
  r.col = (COLS - r.width) / 2;
  return r;
}
