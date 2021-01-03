#include <portmidi.h>
#include "track.h"

#define IS_CHANNEL(msg) (Pm_MessageStatus(msg) < SYSEX)
#define SET_CHANNEL(msg, chan) (((msg) | 0xfffffff0) + (chan))

Track::Track()
  : state(Empty), output(nullptr), channel(0), start_timestamp(0),
    bank_msb(UNDEFINED), bank_lsb(UNDEFINED), prog(UNDEFINED),
    one_shot(false), multi(true)
{
}

Track::Track(Track &other)
  : state(Empty), output(other.output), channel(other.channel), start_timestamp(0),
    bank_msb(other.bank_msb), bank_lsb(other.bank_lsb), prog(other.prog),
    one_shot(other.one_shot), multi(other.multi)
{
}

void Track::record() {
  // TODO
  state = Recording;
}

void Track::overdub() {
  // If recording, send and store note offs for any current note ons.

  // TODO
  state = Overdubbing;
}

void Track::play() {
  if (state == Empty)
    return;

  // If recording or overdubbing, send and store note offs for any current
  // note ons.

  // TODO

  // TODO respect one-shot

  state = Playing;
}

void Track::stop() {
  if (state == Empty)
    return;

  // Send note offs for any current note ons.

  // TODO
  state = Stopped;
}

void Track::clear() {
  events.clear();
  state = Empty;
}

void Track::undo_redo() {
  // TODO
}

void Track::midi_in(PmMessage msg) {
}

// Sends PmEvents to output with changed MIDI channels.
void Track::send(PmEvent *buf, int n) {
  while (n > 0) {
    int num_to_copy = n > 128 ? 128 : n;
    memcpy(event_buffer, buf, num_to_copy * sizeof(PmEvent));
    for (int i = 0; i < num_to_copy; ++i) {
      PmEvent &e = event_buffer[i];
      if (IS_CHANNEL(e.message))
        e.message = SET_CHANNEL(e.message, channel);
    }
    output->write(event_buffer, num_to_copy);
    n -= num_to_copy;
  }
}

void Track::send_program_change() {
  PmEvent events[3];
  int numevents = 0;

  for (int i = 0; i < 3; ++i)
    events[i].timestamp = 0;

  if (bank_msb != UNDEFINED)
    events[numevents++].message = Pm_Message(CONTROLLER + channel, CC_BANK_SELECT_MSB, bank_msb);
  if (bank_lsb != UNDEFINED)
    events[numevents++].message = Pm_Message(CONTROLLER + channel, CC_BANK_SELECT_LSB, bank_lsb);
  if (prog != UNDEFINED)
    events[numevents++].message = Pm_Message(PROGRAM_CHANGE + channel, prog, 0);

  if (numevents > 0)
    output->write(events, numevents);
}
