#include <portmidi.h>
#include "track.h"

#define IS_CHANNEL(msg) (Pm_MessageStatus(msg) < SYSEX)
#define SET_CHANNEL(msg, chan) (((msg) | 0xfffffff0) + (chan))

Track::Track()
  : state(Empty), _output(nullptr), _channel(0), _start_timestamp(0)
{
}

void Track::record() {
  // TODO
  state = Recording;
}

void Track::overdub() {
  // TODO
  state = Overdubbing;
}

void Track::play() {
  // TODO
  state = Playing;
}

void Track::stop() {
  // TODO
  state = Stopped;
}

void Track::clear() {
  // TODO
  _events.clear();
  state = Empty;
}

void Track::midi_in(PmMessage msg) {
}

// Sends PmEvents to output with changed MIDI channels.
void Track::send(PmEvent *buf, int n) {
  while (n > 0) {
    int num_to_copy = n > 128 ? 128 : n;
    memcpy(_event_buffer, buf, num_to_copy * sizeof(PmEvent));
    for (int i = 0; i < num_to_copy; ++i) {
      PmEvent &e = _event_buffer[i];
      if (IS_CHANNEL(e.message))
        e.message = SET_CHANNEL(e.message, _channel);
    }
    _output->write(_event_buffer, num_to_copy);
    n -= num_to_copy;
  }
}
