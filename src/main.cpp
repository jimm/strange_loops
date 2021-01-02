#include <iostream>
#include <iomanip>
#include <getopt.h>
#include <libgen.h>
#include "error.h"
#include "strange_loops.h"
#include "device.h"
#include "curses/gui.h"

struct opts {
  bool list_devices;
  bool testing;
} opts;

void list_devices(const char *title, const PmDeviceInfo *infos[], int num_devices) {
  cout << title << "\n";
  for (int i = 0; i < num_devices; ++i)
    if (infos[i] != nullptr) {
      const char *name = infos[i]->name;
      const char *q = (name[0] == ' ' || name[strlen(name)-1] == ' ') ? "\"" : "";
      cout << "  " << setw(2) << i << ": "
           << q << name << q
           << (infos[i]->opened ? " (open)" : "")
           << "\n";
    }
}

void list_all_devices() {
  int max_instruments = num_devices();
  const PmDeviceInfo *inputs[max_instruments], *outputs[max_instruments];

  for (auto &pair : devices()) {
    int index = pair.first;
    auto info = pair.second;
    inputs[index] = info->input ? info : nullptr;
    outputs[index] = info->output ? info : nullptr;
  }

  list_devices("Inputs", inputs, max_instruments);
  list_devices("Outputs", outputs, max_instruments);
}

void cleanup() {
  stop_portmidi();
}

void initialize() {
  start_portmidi();
  atexit(cleanup);
}

void load(const char *path, bool testing) {
  // Loader loader;
  // if (loader.load(path, testing) == nullptr) // sets PM instance as a side-effect
  //   exit(1);                           // error already printed
  // if (loader.has_error()) {
  //   cerr << "error: " << loader.error() << endl;
  //   exit(1);
  // }
}

void run_curses() {
  StrangeLoops_instance()->start();
  GUI gui(StrangeLoops_instance());
  gui.run();
  // Don't save PM above and use it here. User might have loaded a new one.
  StrangeLoops_instance()->stop();
}

void usage(const char *prog_name) {
  const char * const usage_data[] = {
    "-l or --list-ports",
    "List all attached MIDI ports",
    "-n or --no-midi",
    "No MIDI (ignores bad/unknown MIDI ports)",
    "-h or --help",
    "This help",
  };
  cerr << "usage: " << basename((char *)prog_name) << " [-l] [-n] [-w] [-t] [-v VIEW] [-c] [-h] file\n";
  for (int i = 0; i < sizeof(usage_data) / sizeof(char *); ++i) {
    cerr << endl;
    cerr << "    " << usage_data[i++] << endl;
    cerr << "        " << usage_data[i] << endl;
  }
}

void parse_command_line(int argc, char * const *argv, struct opts *opts) {
  int ch, testing = false;
  char *prog_name = argv[0];
  static struct option longopts[] = {
    {"list", no_argument, 0, 'l'},
    {"no-midi", no_argument, 0, 'n'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  opts->list_devices = opts->testing = false;
  while ((ch = getopt_long(argc, argv, "lnh", longopts, 0)) != -1) {
    switch (ch) {
    case 'l':
      opts->list_devices = true;
      break;
    case 'n':
      opts->testing = true;
      break;
    case 'h': default:
      usage(prog_name);
      exit(ch == '?' || ch == 'h' ? 0 : 1);
    }
  }
}

int main(int argc, char * const *argv) {
  struct opts opts;
  const char *prog_name = argv[0];

  parse_command_line(argc, argv, &opts);
  argc -= optind;
  argv += optind;

  if (opts.list_devices) {
    list_all_devices();
    exit(0);
  }

  initialize();
  if (argc > 0)
    load(argv[0], opts.testing);
  run_curses();

  exit(0);
  return 0;
}
