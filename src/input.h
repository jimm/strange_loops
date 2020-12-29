#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <queue>
#include <mutex>
#include <pthread.h>
#include <portmidi.h>
#include "consts.h"
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
  void read(PmMessage);
  PmMessage message_from_read_queue();
  void stop_read_thread();

protected:
  virtual bool start_midi();

private:
  Track *_track;                // may be null
  bool _running;

  bool is_note_on(PmMessage msg);
  bool is_note_off(PmMessage msg);
  bool is_sustain(PmMessage msg);

  bool notes_on[MIDI_CHANNELS][NOTES_PER_CHANNEL];
  bool sustains_on[MIDI_CHANNELS];
  queue<PmMessage> message_queue;
  mutex message_queue_mutex;
  pthread_t read_pthread;
};

#endif /* INPUT_H */
