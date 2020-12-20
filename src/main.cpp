#include <iostream>
#include <getopt.h>
#include <libgen.h>
#include "error.h"
#include "strange_loops.h"
#include "curses/gui.h"

struct opts {
  bool list_devices;
  bool testing;
} opts;

void list_devices(const char *title, const PmDeviceInfo *infos[], int num_devices) {
  printf("%s:\n", title);
  for (int i = 0; i < num_devices; ++i)
    if (infos[i] != nullptr) {
      const char *name = infos[i]->name;
      const char *q = (name[0] == ' ' || name[strlen(name)-1] == ' ') ? "\"" : "";
      printf("  %2d: %s%s%s%s\n", i, q, name, q, infos[i]->opened ? " (open)" : "");
    }
}

void list_all_devices() {
  int num_devices = Pm_CountDevices();
  const PmDeviceInfo *inputs[num_devices], *outputs[num_devices];

  for (int i = 0; i < num_devices; ++i) {
    const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
    inputs[i] = info->input ? info : 0;
    outputs[i] = info->output ? info : 0;
  }

  list_devices("Inputs", inputs, num_devices);
  list_devices("Outputs", outputs, num_devices);
}

void cleanup() {
  PmError err = Pm_Terminate();
  if (err != 0)
    fprintf(stderr, "error terminating PortMidi: %s\n", Pm_GetErrorText(err));
}

void initialize() {
  PmError err = Pm_Initialize();
  if (err != 0) {
    fprintf(stderr, "error initializing PortMidi: %s\n", Pm_GetErrorText(err));
    exit(1);
  }
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
  // StrangeLoops_instance()->start();
  // GUI gui(StrangeLoops_instance());
  // gui.run();
  // // Don't save PM above and use it here. User might have loaded a new one.
  // StrangeLoops_instance()->stop();
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

  if (argc == 0) {
    usage(prog_name);
    exit(1);
  }

  initialize();
  load(argv[0], opts.testing);
  run_curses();

  exit(0);
  return 0;
}
