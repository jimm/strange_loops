#ifndef TRACKS_WINDOW_H
#define TRACKS_WINDOW_H

#include <vector>
#include "window.h"

class Track;

class TracksWindow : public Window {
public:
  TracksWindow(struct rect, const char *, Track (&tracks)[16]);

  void draw();

private:
  Track (&tracks)[16];

  const char *read_help();
};

#endif /* TRACKS_WINDOW_H */
