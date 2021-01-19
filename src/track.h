#ifndef TRACK_H
#define TRACK_H

#include <string>
#include <vector>
#include <porttime.h>
#include <portmidi.h>
#include "consts.h"
#include "output.h"

enum TrackState {
  Empty,                        // [RecordOverdubPlay] -> recording
  Recording,                    // [RecordOverdubPlay] -> overdubbing
  Overdubbing,                  // [RecordOverdubPlay] -> playing
                                // [StopClear] -> stopped
  Playing,                      // [RecordOverdubPlay] -> overdubbing
                                // [StopClear] -> stopped
  Stopped                       // [RecordOverdubPlay] -> playing
                                // [StopClear] -> empty
};

class Track {
public:
  string name;
  TrackState state;
  Output *output;
  int channel;
  int bank_msb;
  int bank_lsb;
  int prog;
  bool one_shot;
  bool multi;

  Track();
  Track(Track &other);

  void record();
  void overdub();
  void play();
  void stop();
  void clear();
  void undo_redo();

  void midi_in(PmEvent e);
  void send(PmEvent *buf, int n);
  void send_program_change();

private:
  PmTimestamp start_timestamp;
  vector<PmEvent> events;
  PmEvent event_buffer[128];
  bool notes_on[MIDI_CHANNELS][NOTES_PER_CHANNEL];
  bool sustains_on[MIDI_CHANNELS];

  bool is_note_on(PmMessage msg);
  bool is_note_off(PmMessage msg);
  bool is_sustain(PmMessage msg);
};

#endif /* TRACK_H */
