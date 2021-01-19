#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <queue>
#include <mutex>
#include <pthread.h>
#include <portmidi.h>
#include "instrument.h"

using namespace std;

class Track;

class Input : public Instrument {
public:
  Input(PmDeviceID device_id, const char *device_name, const char *name = nullptr);

  virtual bool is_input() { return true; }

  inline bool is_running() { return _running; }

  void start();
  void stop();

  void set_track(Track *track);

  void enqueue(PmEvent *, int);
  void read(PmEvent);
  PmEvent event_from_read_queue();
  void stop_read_thread();

protected:
  virtual bool start_midi();

private:
  Track *_track;                // may be null
  bool _running;

  queue<PmEvent> event_queue;
  mutex event_queue_mutex;
  pthread_t read_pthread;
};

#endif /* INPUT_H */
