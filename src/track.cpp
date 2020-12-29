#include <portmidi.h>
#include "track.h"

Track::Track()
  : state(Empty), start_timestamp(0)
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
  events.clear();
  state = Empty;
}

void Track::midi_in(PmMessage msg) {
}

void Track::send(PmEvent *buf, int n) {
  output->write(buf, n);
}
