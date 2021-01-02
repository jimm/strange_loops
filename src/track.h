#ifndef TRACK_H
#define TRACK_H

#include <string>
#include <porttime.h>
#include <portmidi.h>
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
  std::string name;
  TrackState state;
  Output *output;
  int channel;
  int bank_msb;
  int bank_lsb;
  int prog;

  Track();

  void record();
  void overdub();
  void play();
  void stop();
  void clear();

  void midi_in(PmMessage msg);
  void send(PmEvent *buf, int n);
  void send_program_change();

private:
  PmTimestamp start_timestamp;
  vector<PmEvent> events;
  PmEvent event_buffer[128];
};

#endif /* TRACK_H */
