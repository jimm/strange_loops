#ifndef STRANGE_LOOPS_H
#define STRANGE_LOOPS_H

#include "scene.h"

class StrangeLoops {
public:
  StrangeLoops();

  void panic(bool send_notes_off);

private:
  Scene scenes[128];
  int curr_scene_index;
};

StrangeLoops *StrageLoops_instance();

#endif /* STRANGE_LOOPS_H */
