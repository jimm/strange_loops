#ifndef TRACK_PANEL_H
#define TRACK_PANEL_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
 #include <wx/wx.h>
#endif
#include <wx/tglbtn.h>
#include "../observer.h"

class Track;

class TrackDisplay : public wxPanel {
public:
  Track &track;
  wxButton *track_button;
  int track_num;
  wxChoice *output_instrument_selector;

  TrackDisplay(Track &t, int n) : track(t), track_num(n) {}
};

class TrackPanel : public wxPanel {
public:
  TrackPanel(wxWindow *parent);

  virtual void update(Observable *o, void *arg);
  void update();

private:
  TrackDisplay *track_displays[16];

  wxWindow *make_track_display(Track &track, int track_num);
  void toggle_track(wxCommandEvent& event);

  void on_timer(wxTimerEvent &event);

  wxDECLARE_EVENT_TABLE();
};

#endif /* MONITOR_H */
