#ifndef SCENE_H
#define SCENE_H

#include <portmidi.h>
#include "track.h"

#define NO_ACTIVE_TRACK -1

enum Action {
  AllStartStop,
  UndoRedo,
  TrackRecordOverdubPlay,
  TrackStopClear,
  TrackEdit,
  Save,
};

class Scene {
public:
  Track tracks[16];
  int active_track;
  PmTimestamp scene_length;     // length of first recorded track

  Scene();

  // track_num may be ignored if the action is global.
  void take_action(int track_num, Action action);
};

#endif /* SCENE_H */
