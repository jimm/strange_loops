#include <stdlib.h>
#include <string.h>
#include "help_window.h"

/*
 * Splits `text` into lines and returns a list containing the lines. When
 * you are done with the list, only the first entry should be freed.
 */
vector<char *> *text_to_lines(const char *text) {
  vector<char *> *l = new vector<char *>();

  char *line;
  char *s = strdup(text);
  while ((line = strsep(&s, "\n")) != NULL)
    l->push_back(line);

  return l;
}

void free_lines(vector<char *> *lines) {
  if (lines == nullptr)
    return;

  if (!lines->empty())
    free((*lines)[0]);
  delete lines;
}

HelpWindow::HelpWindow(struct rect r, const char *win_title)
  : Window(r, nullptr)
{
  title = win_title;
  lines = text_to_lines(read_help());
}

HelpWindow::~HelpWindow() {
  if (lines == nullptr)
    return;

  if (!lines->empty())
    free((*lines)[0]);
  delete lines;
}

void HelpWindow::draw() {
  Window::draw();
  int row = 2;
  for (auto& line : *lines) {
    wmove(win, row++, 2);
    waddstr(win, line);
  }
}

const char *HelpWindow::read_help() {
  return
"| Track | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |\n" \
"| Play  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | a  |  s | d  | f  | g  | h  | j  | k  |\n" \
"| Stop  | q | w | e | r | t | y | u | i | z  |  x | c  | v  | b  | n  | m  | ,  |\n" \
"\n" \
"Prefix Commands\n" \
"\n" \
"`  Edit track name, output, channel, program change\n" \
"=  Change BPM (tempo)\n" \
"\n" \
"? - This help\n" \
"\n" \
"ESC  - Panic (ESC ESC sends note-offs)\n" \
"\n" \
"\\    - Quit";
}
