#include <wx/cmdline.h>
#include <portmidi.h>
#include "app.h"
#include "frame.h"
#include "../strange_loops.h"
#include "../device.h"

static const wxCmdLineEntryDesc g_cmdLineDesc [] = {
  { wxCMD_LINE_SWITCH, "l", "list-devices", "Display MIDI Devices" },
  { wxCMD_LINE_PARAM, nullptr, nullptr, "StrangeLoops file", wxCMD_LINE_VAL_STRING,
    wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_NONE }
};

static App *a_instance = nullptr;

App *app_instance() {
  return a_instance;
}

App::App() {
  a_instance = this;
}

App::~App() {
  if (a_instance == this)
    a_instance = nullptr;
}

void App::show_user_message(string msg) {
  frame->show_user_message(msg);
}

void App::show_user_message(string msg, int clear_secs) {
  frame->show_user_message(msg, clear_secs);
}

// This is the wxWidgets equivalent of the "main" function.
bool App::OnInit() {
  if (!wxApp::OnInit())
    return false;

  start_portmidi();
  frame = new Frame("StrangeLoops");
  frame->Show(true);
  SetTopWindow(frame);
  if (command_line_path.IsEmpty())
    frame->initialize();
  else
    frame->load(command_line_path);
  return true;
}

void App::OnInitCmdLine(wxCmdLineParser& parser) {
  parser.SetDesc(g_cmdLineDesc);
  parser.SetSwitchChars (wxT("-"));
}

bool App::OnCmdLineParsed(wxCmdLineParser& parser) {
  if (parser.Found("l")) {
    list_all_devices();
    return false;
  }
  if (parser.GetParamCount() > 0)
    command_line_path = parser.GetParam(0);

  return true;
}

int App::OnExit() {
  StrangeLoops *sloops = StrangeLoops_instance();
  if (sloops) sloops->stop();
  stop_portmidi();
  return wxApp::OnExit();
}

// type is 0 for input, 1 for output
void App::list_all_devices() {
  printf("Inputs:\n");
  for (auto &iter : devices()) {
    const PmDeviceInfo *info = iter.second;
    if (info->input == 1) {
      const char *name = info->name;
      const char *q = (name[0] == ' ' || name[strlen(name)-1] == ' ') ? "\"" : "";
      printf("  %2d: %s%s%s%s\n", iter.first, q, name, q, info->opened ? " (open)" : "");
    }
  }

  printf("Outputs:\n");
  for (auto &iter : devices()) {
    const PmDeviceInfo *info = iter.second;
    if (info->output == 1) {
      const char *name = info->name;
      const char *q = (name[0] == ' ' || name[strlen(name)-1] == ' ') ? "\"" : "";
      printf("  %2d: %s%s%s%s\n", iter.first, q, name, q, info->opened ? " (open)" : "");
    }
  }
}
