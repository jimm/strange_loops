#ifndef TRACK_H
#define TRACK_H

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
  TrackState state;

  Track();

  void record();
  void overdub();
  void play();
  void stop();
  void clear();

  void midi_in(PmMessage msg);
  void send(PmEvent *buf, int n);

private:
  Output *output;
  PmTimestamp start_timestamp;
  vector<PmEvent> events;
};

#endif /* TRACK_H */
