#include <libgen.h>
#include <unistd.h>
#include <wx/defs.h>
#include <wx/filename.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/gbsizer.h>
#include "frame.h"
#include "macros.h"
#include "instrument_dialog.h"
#include "clock_panel.h"
#include "track_panel.h"
#include "../strange_loops.h"

#define LIST_WIDTH 200
#define TALL_LIST_HEIGHT 300
#define SHORT_LIST_HEIGHT 200
#define CLOCK_BPM_HEIGHT 16
#define NOTES_WIDTH 200

wxDEFINE_EVENT(Frame_MenuUpdate, wxCommandEvent);

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
  EVT_MENU(wxID_NEW, Frame::OnNew)
  EVT_MENU(wxID_OPEN, Frame::OnOpen)
  EVT_MENU(wxID_SAVE, Frame::OnSave)
  EVT_MENU(wxID_SAVEAS, Frame::OnSaveAs)
  EVT_MENU(wxID_ABOUT, Frame::OnAbout)
  EVT_MENU(wxID_EXIT, Frame::OnExit)
  EVT_MENU(ID_ListInstruments, Frame::OnListInstruments)
  EVT_MENU(ID_ClockToggle, Frame::toggle_clock)
  EVT_MENU(ID_RegularPanic, Frame::regular_panic)
  EVT_MENU(ID_SuperPanic, Frame::super_panic)

  EVT_COMMAND(wxID_ANY, Frame_MenuUpdate, Frame::update_menu_items)
wxEND_EVENT_TABLE()

void *frame_clear_user_message_thread(void *gui_vptr) {
  Frame *gui = (Frame *)gui_vptr;
  int clear_user_message_id = gui->clear_user_message_id();

  sleep(gui->clear_user_message_seconds());

  // Only clear the window if the id hasn't changed
  if (gui->clear_user_message_id() == clear_user_message_id)
    gui->clear_user_message();
  return nullptr;
}

Frame::Frame(const wxString& title)
  : wxFrame(NULL, wxID_ANY, title), clock_panel(0), track_panel(0)
{
  make_frame_panels();
  make_menu_bar();
  CreateStatusBar();
  create_new_strange_loops();
  show_user_message("No StrangeLoops file loaded", 15);
}

void Frame::make_frame_panels() {
  wxGridBagSizer * const sizer = new wxGridBagSizer();

  sizer->Add(make_clock_panel(this), POS(0, 2), SPAN(1, 1), wxEXPAND);
  sizer->Add(make_track_panel(this), POS(1, 0), SPAN(2, 3), wxEXPAND);

  wxBoxSizer * const outer_border_sizer = new wxBoxSizer(wxVERTICAL);
  outer_border_sizer->Add(sizer, wxSizerFlags().Expand().Border());
  SetSizerAndFit(outer_border_sizer);
}

wxWindow * Frame::make_clock_panel(wxWindow *parent) {
  return clock_panel = new ClockPanel(parent);
}

wxWindow * Frame::make_track_panel(wxWindow *parent) {
  return track_panel = new TrackPanel(parent);
}

void Frame::make_menu_bar() {
  wxMenu *menu_file = new wxMenu;
  menu_file->Append(wxID_NEW, "&New Project\tCtrl-Shift-N", "Create a new project");
  menu_file->Append(wxID_OPEN);
  menu_file->Append(wxID_SAVE);
  menu_file->Append(wxID_SAVEAS);
  menu_file->AppendSeparator();
  menu_file->Append(wxID_EXIT);

  wxMenu *menu_edit = new wxMenu;

  wxMenu *menu_windows = new wxMenu;
  menu_windows->Append(ID_ListInstruments, "&Instruments\tCtrl-I",
                      "Displays input and output instruments");

  wxMenu *menu_midi = new wxMenu;
  menu_midi->Append(ID_ClockToggle, "Start/Stop MIDI Clock\tCtrl-K", "Start or stop the MIDI clock");
  menu_midi->AppendSeparator();
  menu_midi->Append(ID_RegularPanic, "Send &All Notes Off\tCtrl-.",
                   "Send All Notes Off controller message on all channels");
  menu_midi->Append(ID_SuperPanic, "Send Super-&Panic\tCtrl-/",
                   "Send Notes Off messages, all notes, all channels");

  wxMenu *menu_help = new wxMenu;
  menu_help->Append(wxID_ABOUT);

  menu_bar = new wxMenuBar;
  menu_bar->Append(menu_file, "&File");
  menu_bar->Append(menu_edit, "&Edit");
  menu_bar->Append(menu_midi, "&MIDI");
  menu_bar->Append(menu_windows, "&Windows");
  menu_bar->Append(menu_help, "&Help");
  SetMenuBar(menu_bar);
#if defined(__WXMAC__)
  menu_bar->OSXGetAppleMenu()->SetTitle("StrangeLoops");
#endif
}

// ================ messaging ================

void Frame::show_user_message(string msg) {
  SetStatusText(msg.c_str());
}

void Frame::show_user_message(string msg, int secs) {
  // SetStatusText(msg.c_str());
  // clear_user_message_after(secs);
}

void Frame::clear_user_message() {
  SetStatusText("");
}

void Frame::clear_user_message_after(int secs) {
  clear_msg_secs = secs;
  clear_msg_id++;

  pthread_t pthread;
  pthread_create(&pthread, 0, frame_clear_user_message_thread, this);
}

// ================ keypress handling ================

int Frame::handle_global_key_event(wxKeyEvent &event) {
  int key_code = event.GetKeyCode();

  // Do not handle keys if we're in a text control.
  wxString focus_class_name = FindFocus()->GetClassInfo()->GetClassName();
  if (strncmp("wxTextCtrl", focus_class_name.c_str(), 10) == 0)
    return -1;

  // TODO
  switch (event.GetKeyCode()) {
  // case WXK_LEFT: case 'K':
  //   prev_song();
  //   break;
  // case WXK_RIGHT: case 'J':
  //   next_song();
  //   break;
  // case WXK_UP: case 'P':
  //   prev_patch();
  //   break;
  // case WXK_DOWN: case 'N':
  //   next_patch();
  //   break;
  // case ' ':
  //   if (event.ShiftDown())
  //     prev_patch();
  //   else
  //     next_patch();
  //   break;
  default:
    return -1;
  }

  update();
  return true;
}

// ================ MIDI menu actions ================

void Frame::toggle_clock(wxCommandEvent &_event) {
  StrangeLoops_instance()->toggle_clock();
  // clock will send "changed" message which the clock panel will observe.
}

void Frame::regular_panic(wxCommandEvent &_event) {
  StrangeLoops *sloops = StrangeLoops_instance();
  show_user_message("Sending panic...");
  sloops->panic(false);
  show_user_message("Panic sent", 5);
}

void Frame::super_panic(wxCommandEvent &_event) {
  StrangeLoops *sloops = StrangeLoops_instance();
  show_user_message("Sending \"super panic\": all notes off, all channels...");
  sloops->panic(true);
  show_user_message("Super panic sent (all notes off, all channels)", 5);
}

// ================ standard menu items ================

void Frame::OnAbout(wxCommandEvent &_event) {
  wxMessageBox("Strange Loops, the MIDI looper.\n"
               "v1.0.0\n"
               "Jim Menard, jim@jimmenard.com\n"
               "https://github.com/jimm/strange_loops",
               "About StrangeLoops", wxOK | wxICON_INFORMATION);
}

void Frame::OnExit(wxCommandEvent &event) {
  StrangeLoops *sloops = StrangeLoops_instance();
//   if (sloops != nullptr && sloops->is_modified()) {
//     wxMessageDialog dialog(
//       this,
//       "The StrangeLoops data has been modified. Save before quitting?",
//       "Save?",
//       wxYES_NO | wxCANCEL);

//     dialog.SetYesNoCancelLabels("Save", "Quit", "Cancel");
//     switch (dialog.ShowModal()) {
//     case wxID_YES:
//       OnSave(event);
//       goto CLOSE;
//       break;
//     case wxID_NO:
//       goto CLOSE;
//       break;
//     case wxID_CANCEL:
//       return;
//     }
//   }

// CLOSE:
  if (sloops != nullptr) {
    sloops->clock().remove_observer(clock_panel);
    sloops->remove_observer(this);
  }
  Close(true);
}

void Frame::OnNew(wxCommandEvent &_event) {
  create_new_strange_loops();
}

void Frame::OnOpen(wxCommandEvent &_event) {
  wxFileName fname(file_path);

  wxFileDialog openFileDialog(this, "Open StrangeLoops file",
                              fname.GetPath(), fname.GetFullName(),
                              "StrangeLoops files (*.sloops)|*.sloops",
                              wxFD_OPEN|wxFD_FILE_MUST_EXIST);
  if (openFileDialog.ShowModal() != wxID_CANCEL) {
    file_path = openFileDialog.GetPath();
    load(file_path);
  }
  update_menu_items();
}

void Frame::OnSave(wxCommandEvent &event) {
  if (file_path.empty()) {
    OnSaveAs(event);
    return;
  }
  save();
  update_menu_items();
}

void Frame::OnSaveAs(wxCommandEvent &_event) {
  wxFileName fname(file_path);

  wxFileDialog file_dialog(this, "Save StrangeLoops file",
                           fname.GetPath(), fname.GetFullName(),
                           "StrangeLoops files (*.sloops)|*.sloops",
                           wxFD_SAVE);
  if (file_dialog.ShowModal() != wxID_CANCEL) {
    file_path = file_dialog.GetPath();
    save();
    update_menu_items();
  }
}

// ================ windows ================

void Frame::OnListInstruments(wxCommandEvent &_event) {
  InstrumentDialog(this, StrangeLoops_instance()).ShowModal();
  update();
}

// ================ helpers ================

void Frame::initialize() {
  StrangeLoops *sloops = StrangeLoops_instance();
  sloops->start();
  sloops->add_observer(this);
  sloops->clock().add_observer(clock_panel);
  update();
}

void Frame::load(wxString path) {
  // if (access(path, F_OK) != 0) {
  //   wxString err = wxString::Format("File '%s' does not exist", path);
  //   wxLogError(err);
  //   if (StrangeLoops_instance() == nullptr)
  //     create_new_strange_loops();
  //   return;
  // }

  // StrangeLoops *old_sloops = StrangeLoops_instance();

  // Storage storage(path);
  // StrangeLoops *sloops = storage.load();
  // if (storage.has_error()) {
  //   wxLogError("Cannot open file '%s': %s.", path, storage.error());
  //   return;
  // }

  // show_user_message(string(wxString::Format("Loaded %s", path).c_str()), 15);
  // file_path = path;

  // if (old_sloops != nullptr) {
  //   old_sloops->clock().remove_observer(clock_panel);
  //   old_sloops->remove_observer(this);
  //   old_sloops->stop();
  //   delete old_sloops;
  // }
  // sloops->start();                  // initializes cursor
  // sloops->add_observer(this);
  // sloops->clock().add_observer(clock_panel);
  // update();                     // must come after start
}

void Frame::create_new_strange_loops() {
  StrangeLoops *old_sloops = StrangeLoops_instance();
  StrangeLoops *sloops = new StrangeLoops();

  if (old_sloops != nullptr) {
    old_sloops->clock().remove_observer(clock_panel);
    old_sloops->remove_observer(this);
    old_sloops->stop();
    delete old_sloops;
  }

  show_user_message("Created new project", 15);
  file_path = "";
  sloops->start();
  sloops->add_observer(this);
  sloops->clock().add_observer(clock_panel);
  update();                     // must come after start
}

void Frame::save() {
  // if (file_path.empty())
  //   return;

  // Storage storage(file_path);
  // storage.save(StrangeLoops_instance());
  // update();
}

bool Frame::dialog_closed(int dialog_status) {
  bool ok = dialog_status == wxID_OK;
  SetFocus();
  if (ok)
    update();
  return ok;
}

void Frame::update(Observable *o, void *arg) {
  update();
}

void Frame::update() {
  clock_panel->update();
  update_title();
  update_menu_items();
}

void Frame::update_title() {
  StrangeLoops *sloops = StrangeLoops_instance();
  bool modified = sloops != nullptr && sloops->is_modified();
  char *file_name;

  if (file_path.empty())
    file_name = (char *)"StrangeLoops";
  else {
    char path[BUFSIZ];
    strcpy(path, file_path.c_str());
    file_name = basename(path);
  }
  SetTitle(wxString::Format("%s%s", modified ? "* " : "", file_name));
}

void Frame::update_menu_items() {
  StrangeLoops *sloops = StrangeLoops_instance();

  // file menu
  menu_bar->FindItem(wxID_SAVEAS, nullptr)->Enable(!file_path.empty());
}
