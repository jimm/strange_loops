#ifndef SCENE_H
#define SCENE_H

#include <portmidi.h>
#include "track.h"

#define NUM_TRACKS 16
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

  void initialize(Output *default_output);

  // track_num may be ignored if the action is global.
  void take_action(int track_num, Action action);

  void all_tracks_send(PmEvent *buf, int num_events);

protected:
  Track _tracks[NUM_TRACKS];
  PmTimestamp _scene_length;    // length of first recorded track

  void all_start_stop();
  void undo_redo(int track_num);
  void track_record_overdub_play(int track_num);
  void track_stop_clear(int track_num);
  void track_edit(int track_num);
  void save();
};

#endif /* SCENE_H */
