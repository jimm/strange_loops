#include <portmidi.h>
#include "strange_loops.h"
#include "device.h"

static StrangeLoops *sl_instance = nullptr;

StrangeLoops *StrangeLoops_instance() {
  if (sl_instance == nullptr) {
    sl_instance = new StrangeLoops();
    sl_instance->initialize();
  }
  return sl_instance;
}

StrangeLoops::StrangeLoops()
  : _curr_scene_index(0), _clock(_inputs), _modified(false)
{
  for (auto &iter : devices()) {
    const PmDeviceInfo *info = iter.second;
    if (info->input)
      _inputs.push_back(new Input(iter.first, info->name));
    if (info->output)
      _outputs.push_back(new Output(iter.first, info->name));
  }

  sl_instance = this;
}

void StrangeLoops::initialize() {
  for (int i = 0; i < NUM_SCENES; ++i)
    _scenes[i].initialize(_outputs[0]);
}

void StrangeLoops::panic(bool send_notes_off) {
  PmEvent buf[128];
  Scene &curr_scene = _scenes[_curr_scene_index];

  memset(buf, 0, 128 * sizeof(PmEvent));
  if (send_notes_off) {
    for (int i = 0; i < 128; ++i)
      buf[i].timestamp = 0;
    for (int i = 0; i < 16; ++i) {
      for (int j = 0; j < 128; ++j)
        buf[j].message = Pm_Message(NOTE_OFF + i, j, 0);
      curr_scene.all_tracks_send(buf, 128);
    }
  }
  else {
    for (int i = 0; i < 16; ++i) {
      buf[i].message = Pm_Message(CONTROLLER + i, CM_ALL_NOTES_OFF, 0);
      buf[i].timestamp = 0;
    }
    curr_scene.all_tracks_send(buf, 16);
  }
}

void StrangeLoops::start() {
  update_clock();
}

void StrangeLoops::stop() {
  stop_clock();
}

// ================ clock ================

void StrangeLoops::update_clock() {
  Scene &curr_scene = current_scene();
  set_clock_bpm(curr_scene.bpm());
  start_clock();
}

// ================ observer / observable ================

// Only called by storage after data is loaded or saved.
void StrangeLoops::clear_modified() {
  _modified = false;
  Observable::changed((void *)&_modified);
}

void StrangeLoops::update(Observable *_o, void *_arg) {
  changed();
}

void StrangeLoops::changed(void *_arg) {
  _modified = true;
  Observable::changed((void *)&_modified);
}
