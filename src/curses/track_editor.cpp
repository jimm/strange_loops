#include <stdlib.h>
#include <string.h>
#include "track_editor.h"

TrackEditor::TrackEditor(struct rect r, const char *win_title,
                         vector<Output *>&outputs_ref, Track &track_ref)
  : Window(r, nullptr), outputs(outputs_ref), track(track_ref)
{
  title = win_title;
}

void TrackEditor::run() {
  bool done = FALSE;
  int ch;

  while (!done) {
    draw();
    wrefresh(win);

    ch = getch();
    switch (ch) {
    case '\e':
      done = true;
      break;
    }
  }
}

void TrackEditor::draw() {
  char buf[BUFSIZ];

  Window::draw();
  int row = 2;

  wmove(win, 2, 2);
  waddstr(win, "Output:");
  row++;
  for (auto &output : outputs) {
    sprintf(buf, " %-32s ", output->name.c_str());
    wmove(win, row++, 4);
    if (output == track.output)
      wattron(win, A_REVERSE);
    waddstr(win, buf);
    if (output == track.output)
      wattroff(win, A_REVERSE);
  }

  wmove(win, 2, 40);
  waddstr(win, "Channel:");
  for (int chan = 0; chan < 16; ++chan) {
    sprintf(buf, " %2d ", chan);
    wmove(win, chan + 3, 42);
    if (chan == track.channel)
      wattron(win, A_REVERSE);
    waddstr(win, buf);
    if (chan == track.channel)
      wattroff(win, A_REVERSE);
  }

  input_field("Bank MSB", track.bank_msb, 3);
  input_field("Bank LSB", track.bank_lsb, 5);
  input_field(" Program", track.prog, 7);
}

void TrackEditor::input_field(const char * const field_name, int val, int row) {
  char buf[8];

  if (val == UNDEFINED)
    strcpy(buf, "     ");
  else
    sprintf(buf, " %3d ", val);
  wmove(win, row, 50);
  waddstr(win, field_name);
  waddstr(win, ":  ");
  wattron(win, A_REVERSE);
  waddstr(win, buf);
  wattroff(win, A_REVERSE);
}

