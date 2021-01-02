#include "scene.h"

Scene::Scene()
  : _scene_length(0L), _bpm(120.0)
{
}

void Scene::initialize(Output *default_output) {
  for (int i = 0; i < NUM_TRACKS; ++i) {
    Track &t = _tracks[i];
    string name = "Track ";
    name += std::to_string(i + 1);

    t.name = name;
    t.output =  default_output;
    t.channel = i;
  }
}

void Scene::set_bpm(float bpm) {
  if (_bpm != bpm) {
    _bpm = bpm;
    // changed();
  }
}

void Scene::take_action(int track_num, Action action) {
  switch (action) {
  case AllStartStop:
    for (int i = 0; i < NUM_TRACKS; ++i)
      _tracks[i].stop();
    break;
  case UndoRedo:
    // TODO
    break;
  case TrackRecordOverdubPlay:
    for (int i = 0; i < NUM_TRACKS; ++i)
      if (i != track_num && (_tracks[i].state == Recording || _tracks[i].state == Overdubbing))
        _tracks[i].play();

    switch (_tracks[track_num].state) {
    case Empty:
      _tracks[track_num].record();
      break;
    case Recording: case Playing:
      _tracks[track_num].overdub();
      break;
    case Overdubbing: case Stopped:
      _tracks[track_num].play();
      break;
    }
    break;
  case TrackStopClear:
    switch (_tracks[track_num].state) {
    case Overdubbing: case Playing:
      _tracks[track_num].stop();
      break;
    case Stopped:
      _tracks[track_num].clear();
      break;
    default:
      break;
    }
    break;
  case TrackEdit:
    // TODO
    break;
  case Save:
    break;
  }
}

void Scene::all_tracks_send(PmEvent *buf, int num_events) {
  for (auto &track : _tracks)
    track.send(buf, num_events);
}
