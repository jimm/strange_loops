#include "track_panel.h"
#include "macros.h"
#include "../strange_loops.h"
#include "../track.h"

enum {
  ID_TrackBPM = 200,
  ID_TrackToggle
};

wxBEGIN_EVENT_TABLE(TrackPanel, wxPanel)
wxEND_EVENT_TABLE()


TrackPanel::TrackPanel(wxWindow *parent)
: wxPanel(parent, wxID_ANY)
{     
  Scene &scene = StrangeLoops_instance()->current_scene();
  wxBoxSizer *outer_sizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *inner_sizer = new wxBoxSizer(wxHORIZONTAL);

  for (int i = 0; i < 16; ++i) {
    if (i  == 8) {
      outer_sizer->Add(inner_sizer);
      inner_sizer = new wxBoxSizer(wxHORIZONTAL);
    }
    wxWindow *win = make_track_display(scene.track(i), i + 1);
    inner_sizer->Add(win);
  }
  outer_sizer->Add(inner_sizer);

  SetSizerAndFit(outer_sizer);
  update();
}

wxWindow *TrackPanel::make_track_display(Track &track, int track_num) {
  wxPanel *panel = new wxPanel(this);
  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

  TrackDisplay *td = new TrackDisplay(track, track_num + 1);
  track_displays[track_num] = td;
  td->track_button = new wxButton(panel, wxID_ANY, wxString::Format("%d", track_num + 1));

  td->output_instrument_selector = new wxChoice(panel, wxID_ANY);
  for (auto &output : StrangeLoops_instance()->outputs())
    td->output_instrument_selector->AppendString(output->name);

  sizer->Add(td->track_button);
  sizer->Add(td->output_instrument_selector);
  panel->SetSizerAndFit(sizer);
  return panel;
}

void TrackPanel::update(Observable *o, void *arg) {
  // TODO
}

void TrackPanel::update() {
  StrangeLoops *sloops = StrangeLoops_instance();
  if (sloops == nullptr)
    return;

  for (int i = 0; i < 16; ++i) {
    // TODO
    TrackDisplay *td = track_displays[i];
  }
}
