#include "scene.h"

Scene::Scene()
  : active_track(-1), scene_length(0L)
{
}

void Scene::take_action(int track_num, Action action) {
  Track *track;

  switch (action) {
  case AllStartStop:
    for (int i = 0; i < 16; ++i)
      tracks[i].stop();
    active_track = NO_ACTIVE_TRACK;
    break;
  case UndoRedo:
    break;
  case TrackRecordOverdubPlay:
    track = &tracks[track_num];
    break;
  case TrackStopClear:
    track = &tracks[track_num];
    break;
  case TrackEdit:
    track = &tracks[track_num];
    break;
  case Save:
    break;
  }
}
