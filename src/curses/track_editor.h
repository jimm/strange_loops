#ifndef TRACK_EDITOR_H
#define TRACK_EDITOR_H

#include <string>
#include <vector>
#include "window.h"
#include "../strange_loops.h"

class Output;

enum TEActiveField {
  TEAF_Name, TEAF_OneShot, TEAF_Multi, TEAF_Output, TEAF_Chan, TEAF_MSB, TEAF_LSB, TEAF_Prog
};

class TrackEditor : public Window {
public:
  TrackEditor(struct rect, const char *win_title, vector <Output *> &outputs, Track &track);

  void run();
  void draw();

private:
  vector <Output *> &outputs;
  Track &track;
  Track edit_track;
  int output_index;
  TEActiveField active_field;

  void move_up();
  void move_down();
  void move_left();
  void move_right();
  void next_field();
  void prev_field();

  void draw_name();
  void draw_one_shot();
  void draw_multi();
  void draw_outputs();
  void draw_channels();
  void draw_input_fields();

  void save();
  void input_field(const char * const field_name, int val, int row, bool cursor_here);
  void edit_number(int ch);
  void edit_name(int ch);
};

#endif /* TRACK_EDITOR_H */
