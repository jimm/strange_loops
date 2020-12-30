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
  Scene();

  Track &track(int i) { return _tracks[i]; }
  Track &current_track() { return track(_active_track_index); }

  void set_bpm(float bpm);
  float bpm() { return _bpm; }

  // track_num may be ignored if the action is global.
  void take_action(int track_num, Action action);

  void all_tracks_send(PmEvent *buf, int num_events);

protected:
  Track _tracks[16];
  int _active_track_index;
  float _bpm;
  PmTimestamp _scene_length;    // length of first recorded track

};

#endif /* SCENE_H */
