#ifndef SCENE_H
#define SCENE_H

#include <portmidi.h>
#include "track.h"

class Scene {
public:
  Track tracks[16];
  int active_track;
  PmTimestamp scene_length;     // length of first recorded track

  Scene();
};

#endif /* SCENE_H */
