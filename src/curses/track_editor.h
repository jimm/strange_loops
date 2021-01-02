#ifndef TRACK_EDITOR_H
#define TRACK_EDITOR_H

#include <vector>
#include "window.h"
#include "../track.h"

class TrackEditor : public Window {
public:
  vector<char *> *lines;

  TrackEditor(struct rect, const char *, Track &track);

  void draw();

private:
  Track &track;

};

#endif /* TRACK_EDITOR_H */
