#ifndef WX_FRAME_H
#define WX_FRAME_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
 #include <wx/wx.h>
 #include <wx/listctrl.h>
#endif
#include "events.h"
#include "../strange_loops.h"
#include "../observer.h"

enum {
  ID_ListInstruments=1,
  ID_ClockToggle,
  ID_RegularPanic,
  ID_SuperPanic
};

class wxTextCtrl;
class ClockPanel;
class TrackPanel;

class Frame: public wxFrame, public Observer {
public:
  Frame(const wxString& title);
  virtual ~Frame() {}

  void initialize();
  void load(wxString path);
  void save();

  int handle_global_key_event(wxKeyEvent &event);


  virtual void update(Observable *o, void *arg);
  void update(wxCommandEvent& event) { update(); }
  void update();

  void update_menu_items(wxCommandEvent& event) { update_menu_items(); }
  void update_menu_items();

  void show_user_message(std::string);
  void show_user_message(std::string, int);
  void clear_user_message();
  void clear_user_message_after(int);
  int clear_user_message_seconds() { return clear_msg_secs; }
  int clear_user_message_id() { return clear_msg_id; }

private:
  wxString file_path;
  wxMenuBar *menu_bar;
  ClockPanel *clock_panel;
  TrackPanel *track_panel;
  int clear_msg_secs;
  int clear_msg_id;

  void OnNew(wxCommandEvent& event);
  void OnOpen(wxCommandEvent& event);
  void OnSave(wxCommandEvent& event);
  void OnSaveAs(wxCommandEvent& event);
  void OnListInstruments(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);

  void create_new_strange_loops();

  void toggle_clock(wxCommandEvent &_event);
  void regular_panic(wxCommandEvent &_event);
  void super_panic(wxCommandEvent &_event);

  void make_frame_panels();
  void make_menu_bar();
  wxWindow * make_clock_panel(wxWindow *);
  wxWindow * make_track_panel(wxWindow *);

  bool dialog_closed(int dialog_return_code);
  void update_title();

  wxDECLARE_EVENT_TABLE();
};

#endif /* WX_FRAME_H */
