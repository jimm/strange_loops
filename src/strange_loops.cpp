#include <portmidi.h>
#include "strange_loops.h"

static StrangeLoops *sl_instance = nullptr;

StrangeLoops *StrangeLoops_instance() {
  return sl_instance;
}

StrangeLoops::StrangeLoops()
  : curr_scene_index(0)
{
  sl_instance = this;
}

void StrangeLoops::panic(bool send_notes_off) {
  PmEvent buf[128];

  memset(buf, 0, 128 * sizeof(PmEvent));
  if (send_notes_off) {
    for (int i = 0; i < 128; ++i)
      buf[i].timestamp = 0;
    for (int i = 0; i < 16; ++i) {
      for (int j = 0; j < 128; ++j)
        buf[j].message = Pm_Message(NOTE_OFF + i, j, 0);
      for (auto &track : scenes[curr_scene_index].tracks)
        track.send(buf, 128);
    }
  }
  else {
    for (int i = 0; i < 16; ++i) {
      buf[i].message = Pm_Message(CONTROLLER + i, CM_ALL_NOTES_OFF, 0);
      buf[i].timestamp = 0;
    }
    for (auto &track : scenes[curr_scene_index].tracks)
      track.send(buf, 16);
  }
}
