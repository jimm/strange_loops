#ifndef TRACK_H
#define TRACK_H

#include <porttime.h>
#include <portmidi.h>
#include "output.h"

enum TrackButtonActions {
  RecordOverdubPlay,
  StopClear
};

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
  Output *output;
  TrackState state;
  PmTimestamp start_timestamp;
  vector<PmEvent> events;

  Track();

  void midi_in(PmMessage msg);
};

#endif /* TRACK_H */
