#include "scene.h"

Scene::Scene()
  : _scene_length(0L)
{
}

void Scene::initialize(Output *default_output) {
  for (int i = 0; i < NUM_TRACKS; ++i) {
    Track &t = _tracks[i];
    string name = "Trk ";
    name += std::to_string(i + 1);

    t.name = name;
    t.output =  default_output;
    t.channel = i;
  }
}

void Scene::take_action(int track_num, Action action) {
  switch (action) {
  case AllStartStop:
    all_start_stop();
    break;
  case UndoRedo:
    undo_redo(track_num);
    break;
  case TrackRecordOverdubPlay:
    track_record_overdub_play(track_num);
    break;
  case TrackStopClear:
    track_stop_clear(track_num);
    break;
  case TrackEdit:
    track_edit(track_num);
    break;
  case Save:
    save();
    break;
  }
}

void Scene::all_start_stop() {
  bool all_stopped_or_empty = true;

  for (int i = 0; i < NUM_TRACKS && all_stopped_or_empty; ++i)
    all_stopped_or_empty &= (_tracks[i].state == Empty || _tracks[i].state == Stopped);
  for (int i = 0; i < NUM_TRACKS; ++i) {
    if (all_stopped_or_empty)
      _tracks[i].play();
    else
      _tracks[i].stop();
  }
}

void Scene::undo_redo(int track_num) {
  _tracks[track_num].undo_redo();
}

void Scene::track_record_overdub_play(int track_num) {
  // All other recording or overdubbing tracks should switch to play
  for (int i = 0; i < NUM_TRACKS; ++i)
    if (i != track_num && (_tracks[i].state == Recording || _tracks[i].state == Overdubbing))
      _tracks[i].play();

  switch (_tracks[track_num].state) {
  case Empty:
    // TODO set inputs' track to this track
    _tracks[track_num].record();
    break;
  case Recording: case Playing:
    // TODO set inputs' track to this track
    _tracks[track_num].overdub();
    break;
  case Overdubbing: case Stopped:
    // TODO set inputs' track to nullptr
    _tracks[track_num].play();
  default:
    // TODO set inputs' track to nullptr
    break;
  }
}

void Scene::track_stop_clear(int track_num) {
  switch (_tracks[track_num].state) {
  case Recording: case Overdubbing: case Playing:
    _tracks[track_num].stop();
    break;
  case Stopped:
    _tracks[track_num].clear();
    break;
  default:
    break;
  }
}

void Scene::track_edit(int track_num) {
  // TODO
}

void Scene::save() {
  // TODO
}

void Scene::all_tracks_send(PmEvent *buf, int num_events) {
  for (auto &track : _tracks)
    track.send(buf, num_events);
}
