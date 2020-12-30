#include "scene.h"

Scene::Scene()
  : _active_track_index(-1), _scene_length(0L), _bpm(120.0)
{
}

void Scene::set_bpm(float bpm) {
  if (_bpm != bpm) {
    _bpm = bpm;
    // changed();
  }
}

void Scene::take_action(int track_num, Action action) {
  Track *track;

  switch (action) {
  case AllStartStop:
    for (int i = 0; i < 16; ++i)
      _tracks[i].stop();
    _active_track_index = NO_ACTIVE_TRACK;
    break;
  case UndoRedo:
    break;
  case TrackRecordOverdubPlay:
    track = &_tracks[track_num];
    break;
  case TrackStopClear:
    track = &_tracks[track_num];
    break;
  case TrackEdit:
    track = &_tracks[track_num];
    break;
  case Save:
    break;
  }
}

void Scene::all_tracks_send(PmEvent *buf, int num_events) {
  for (auto &track : _tracks)
    track.send(buf, num_events);
}
