#ifndef TRACK_EDITOR_H
#define TRACK_EDITOR_H

#include <vector>
#include "window.h"
#include "../strange_loops.h"

class TrackEditor : public Window {
public:
  vector<char *> *lines;

  TrackEditor(struct rect, const char *, vector <Output *> &outputs, Track &track);

  void run();
  void draw();

private:
  vector <Output *> &outputs;
  Track &track;

  void input_field(const char * const field_name, int val, int row);
};

#endif /* TRACK_EDITOR_H */
