#include <stdlib.h>
#include <string.h>
#include <portmidi.h>
#include "error.h"
#include "output.h"

Output::Output(PmDeviceID device_id, const char *device_name, const char *name)
  : Instrument(device_id, device_name, name)
{
}

bool Output::start_midi() {
  PmError err = Pm_OpenOutput(&stream, device_id, 0, 128, 0, 0, 0);
  if (err == 0)
    return true;

  char buf[BUFSIZ];
  sprintf(buf, "error opening output stream %s: %s\n", name.c_str(),
          Pm_GetErrorText(err));
  error_message(buf);
  return false;
}

void Output::write(PmEvent *buf, int len) {
  if (real_port()) {
    if (enabled) {
      output_mutex.lock();
      PmError err = Pm_Write(stream, buf, len);
      output_mutex.unlock();
      if (err != 0) {
        char err_msg_buf[BUFSIZ];
        sprintf(err_msg_buf, "error writing MIDI to %s: %s\n",
                name.c_str(), Pm_GetErrorText(err));
        error_message(err_msg_buf);
        for (int i = 0; i < len; ++i)
          fprintf(stderr, "msg %d\t%08x\n", i,buf[i].message);
      }
    }
  }
  else {
    for (int i = 0; i < len && num_io_messages < MIDI_BUFSIZ-1; ++i)
      io_messages[num_io_messages++] = buf[i].message;
  }
}
