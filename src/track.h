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

  Output *output() { return _output; }
  int channel() { return _channel; }

  void set_output(Output *output) { _output = output; }
  void set_channel(int channel) { _channel = channel; }

  void midi_in(PmMessage msg);
  void send(PmEvent *buf, int n);

private:
  Output *_output;
  int _channel;
  PmTimestamp _start_timestamp;
  vector<PmEvent> _events;
  PmEvent _event_buffer[128];
};

#endif /* TRACK_H */
