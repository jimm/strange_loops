#ifndef STRANGE_LOOPS_H
#define STRANGE_LOOPS_H

#include <vector>
#include "observer.h"
#include "observable.h"
#include "scene.h"

#define NUM_SCENES 128

class Input;
class Output;

class StrangeLoops : public Observer, public Observable {
public:
  StrangeLoops();

  void initialize();

  void panic(bool send_notes_off);

  void start();
  void stop();

  // ================ accessors ================
  inline vector<Input *> &inputs() { return _inputs; }
  inline vector<Output *> &outputs() { return _outputs; }
  Scene &current_scene() { return _scenes[_curr_scene_index]; }
  inline bool is_modified() { return _modified; }

  // ================ observer / observable ================
  // Only called by storage after data is loaded or saved.
  void clear_modified();

private:
  vector<Input *> _inputs;
  vector<Output *> _outputs;
  Scene _scenes[128];
  int _curr_scene_index;
  bool _modified;

  // ================ observer / observable ================
  void update(Observable *o, void *arg);
  // Call the public method clear_modified() to reset _modified to false
  void changed(void *arg = nullptr);
};

StrangeLoops *StrangeLoops_instance();

#endif /* STRANGE_LOOPS_H */
