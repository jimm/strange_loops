#include "tracks_window.h"
#include "../track.h"

TracksWindow::TracksWindow(struct rect r, const char *win_title, Track (&track_array)[16])
  : Window(r, nullptr), tracks(track_array)
{
  title = win_title;
}

void TracksWindow::draw() {
  Window::draw();
  for (int i = 0; i < 16; ++i) {
    wattron(win, A_REVERSE);
    wmove(win, 0, i * 10 + 2);
    waddstr(win, "        ");
    waddstr(win, "   ");
    waddch(win, i < 10 ? ' ' : '0' + (i / 10));
    waddch(win, '0' + (i % 10));
    waddstr(win, "     ");
    waddstr(win, "        ");
    wattroff(win, A_REVERSE);
  }
}
