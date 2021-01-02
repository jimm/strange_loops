#ifndef DEVICE_H
#define DEVICE_H

#include <map>
#include "portmidi.h"

void start_portmidi();
void stop_portmidi();

int num_devices();
const std::map<int, const PmDeviceInfo *> &devices();
PmDeviceID find_device(const char *device_name, int device_type);
bool device_names_equal(const char *name1, const char *name2);

#endif /* DEVICE_H */