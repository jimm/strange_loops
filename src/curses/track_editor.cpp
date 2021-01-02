#include <stdlib.h>
#include <string.h>
#include "track_editor.h"

TrackEditor::TrackEditor(struct rect r, const char *win_title, Track &track_ref)
  : Window(r, nullptr), track(track_ref)
{
  title = win_title;
}

void TrackEditor::draw() {
  Window::draw();
  int row = 2;
  // TODO
}
