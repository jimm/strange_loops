#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <string>
#include <portmidi.h>

#define MIDI_BUFSIZ 128

class Instrument {
public:
  PmDeviceID device_id;
  std::string name;
  std::string device_name;
  PortMidiStream *stream;
  bool enabled;

  PmEvent io_events[MIDI_BUFSIZ]; // testing only
  int num_io_events;              // ditto

  Instrument(PmDeviceID device_id, const char *device_name,
             const char *name = nullptr);
  virtual ~Instrument() {}

  virtual bool is_input() { return false; }
  virtual bool is_output() { return false; }

  virtual void start();
  virtual void stop();
  bool real_port();

  void clear();                 // testing only

protected:
  virtual bool start_midi() { return false; }
  virtual void stop_midi();
};

#endif /* INSTRUMENT_H */
