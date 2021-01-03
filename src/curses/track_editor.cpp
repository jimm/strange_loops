#include "track_editor.h"

#define DELETE_KEY 127

TrackEditor::TrackEditor(struct rect r, const char *win_title,
                         vector<Output *>&outputs_ref, Track &track_ref)
  : Window(r, nullptr), outputs(outputs_ref), track(track_ref), edit_track(track_ref),
    active_field(TEAF_Name)
{
  title = win_title;
  hint = "Return = OK, \\ = Cancel";
  for (int i = 0; i < outputs.size(); ++i)
    if (outputs[i] == edit_track.output) {
      output_index = i;
      break;
    }
}

void TrackEditor::run() {
  bool done = FALSE;
  int ch;

  while (!done) {
    draw();
    wrefresh(win);

    ch = getch();
    switch (ch) {
    case KEY_UP:
      move_up();
      break;
    case KEY_DOWN:
      move_down();
      break;
    case KEY_LEFT:
      move_left();
      break;
    case KEY_RIGHT:
      move_right();
      break;
    case '\t':
      next_field();
      break;
    case KEY_BTAB:
      prev_field();
      break;
    case '\\':
      done = true;
      break;
    case '\n':
      save();
      done = true;
      break;
    case '?':
      // TODO
      break;
    default:
      switch (active_field) {
      case TEAF_Name:
        edit_name(ch);
        break;
      case TEAF_OneShot:
        if (ch == 'y')
          edit_track.one_shot = true;
        else if (ch == 'n')
          edit_track.one_shot = false;
        break;
      case TEAF_Multi:
        if (ch == 'y')
          edit_track.multi = true;
        else if (ch == 'n')
          edit_track.multi = false;
        break;
      case TEAF_MSB: case TEAF_LSB: case TEAF_Prog:
        edit_number(ch);
        break;
      default:
        break;
      }
      break;
    }
  }
  curs_set(0);
}

void TrackEditor::move_up() {
  switch (active_field) {
  case TEAF_Name:
  case TEAF_OneShot:
  case TEAF_Multi:
    break;
  case TEAF_Output:
    if (output_index > 0)
      edit_track.output = outputs[--output_index];
    else
      active_field = TEAF_Name;
    break;
  case TEAF_Chan:
    if (edit_track.channel > 0)
      --edit_track.channel;
    else
      active_field = TEAF_OneShot;
    break;
  case TEAF_MSB:
    active_field = TEAF_Multi;
    break;
  case TEAF_LSB:
    active_field = TEAF_MSB;
    break;
  case TEAF_Prog:
    active_field = TEAF_LSB;
    break;
  }
}

void TrackEditor::move_down() {
  switch (active_field) {
  case TEAF_Name:
    active_field = TEAF_Output;
    break;
  case TEAF_OneShot:
    active_field = TEAF_Chan;
    break;
  case TEAF_Multi:
    active_field = TEAF_MSB;
    break;
  case TEAF_Output:
    if (output_index < outputs.size() - 1)
      edit_track.output = outputs[++output_index];
    break;
  case TEAF_Chan:
    if (edit_track.channel < 16 - 1)
      ++edit_track.channel;
    break;
  case TEAF_MSB:
    active_field = TEAF_LSB;
    break;
  case TEAF_LSB:
    active_field = TEAF_Prog;
    break;
  case TEAF_Prog:
    break;
  }
}

void TrackEditor::move_left() {
  switch (active_field) {
  case TEAF_Name:
    break;
  case TEAF_OneShot:
    active_field = TEAF_Name;
    break;
  case TEAF_Multi:
    active_field = TEAF_OneShot;
    break;
  case TEAF_Output:
    break;
  case TEAF_Chan:
    active_field = TEAF_Output;
    break;
  case TEAF_MSB:
  case TEAF_LSB:
  case TEAF_Prog:
    active_field = TEAF_Chan;
    break;
  }
}

void TrackEditor::move_right() {
  switch (active_field) {
  case TEAF_Name:
    active_field = TEAF_OneShot;
    break;
  case TEAF_OneShot:
    active_field = TEAF_Multi;
    break;
  case TEAF_Multi:
    break;
  case TEAF_Output:
    active_field = TEAF_Chan;
    break;
  case TEAF_Chan:
    active_field = TEAF_MSB;
    break;
  case TEAF_MSB:
    break;
  case TEAF_LSB:
    break;
  case TEAF_Prog:
    break;
  }
}

void TrackEditor::next_field() {
  switch (active_field) {
  case TEAF_Name:
    active_field = TEAF_OneShot;
    break;
  case TEAF_OneShot:
    active_field = TEAF_Multi;
    break;
  case TEAF_Multi:
    active_field = TEAF_Output;
    break;
  case TEAF_Output:
    active_field = TEAF_Chan;
    break;
  case TEAF_Chan:
    active_field = TEAF_MSB;
    break;
  case TEAF_MSB:
    active_field = TEAF_LSB;
    break;
  case TEAF_LSB:
    active_field = TEAF_Prog;
    break;
  case TEAF_Prog:
    active_field = TEAF_Name;
    break;
  }
}

void TrackEditor::prev_field() {
  switch (active_field) {
  case TEAF_Name:
    active_field = TEAF_Prog;
    break;
  case TEAF_OneShot:
    active_field = TEAF_Name;
    break;
  case TEAF_Multi:
    active_field = TEAF_OneShot;
    break;
  case TEAF_Output:
    active_field = TEAF_Multi;
    break;
  case TEAF_Chan:
    active_field = TEAF_Output;
    break;
  case TEAF_MSB:
    active_field = TEAF_Chan;
    break;
  case TEAF_LSB:
    active_field = TEAF_MSB;
    break;
  case TEAF_Prog:
    active_field = TEAF_LSB;
    break;
  }
}

void TrackEditor::draw() {
  Window::draw();
  draw_name();
  draw_one_shot();
  draw_multi();
  draw_outputs();
  draw_channels();
  draw_input_fields();
}

void TrackEditor::draw_name() {
  wmove(win, 2, 1);
  waddstr(win, active_field == TEAF_Name ? "->" : "  ");
  waddstr(win, "Name: ");

  wattron(win, A_REVERSE);
  wprintw(win, "%-16s", edit_track.name.c_str());
  // FIXME blinking cursor
  // if (active_field == TEAF_Name) {
  //   wmove(win, 2, 7 + edit_track.name.size());
  //   wattron(win, A_BLINK);
  //   waddch(win, ' ');
  //   wattroff(win, A_BLINK);
  // }
  wattroff(win, A_REVERSE);
}

void TrackEditor::draw_one_shot() {
  wmove(win, 2, 26);
  waddstr(win, active_field == TEAF_OneShot ? "->" : "  ");
  waddstr(win, "One-Shot: ");

  if (edit_track.one_shot)
    wattron(win, A_REVERSE);
  waddstr(win, " Yes ");
  if (edit_track.one_shot)
    wattroff(win, A_REVERSE);

  waddch(win, '/');

  if (!edit_track.one_shot)
    wattron(win, A_REVERSE);
  waddstr(win, " No  ");
  if (!edit_track.one_shot)
    wattroff(win, A_REVERSE);
}

void TrackEditor::draw_multi() {
  wmove(win, 2, 48);
  waddstr(win, active_field == TEAF_Multi ? "->" : "  ");
  waddstr(win, "Multi: ");

  if (edit_track.multi)
    wattron(win, A_REVERSE);
  waddstr(win, " Yes ");
  if (edit_track.multi)
    wattroff(win, A_REVERSE);

  waddch(win, '/');

  if (!edit_track.multi)
    wattron(win, A_REVERSE);
  waddstr(win, " No  ");
  if (!edit_track.multi)
    wattroff(win, A_REVERSE);
}

void TrackEditor::draw_outputs() {
  int row, idx;

  row = 4;
  wmove(win, row++, 2);
  waddstr(win, "Output:");
  for (auto &o : outputs) {
    wmove(win, row, 4);
    if (o == edit_track.output)
      wattron(win, A_REVERSE);
    wprintw(win, " %-32s ", o->name.c_str());
    if (o == edit_track.output)
      wattroff(win, A_REVERSE);

    if (active_field == TEAF_Output && o == edit_track.output) {
      wmove(win, row, 2);
      waddstr(win, "->");
    }
    ++row;
    ++idx;
  }
}

void TrackEditor::draw_channels() {
  int row, idx;

  row = 4;
  wmove(win, row++, 40);
  waddstr(win, "Channel:");
  for (int chan = 0; chan < 16; ++chan) {
    wmove(win, row, 42);
    if (chan == edit_track.channel)
      wattron(win, A_REVERSE);
    wprintw(win, " %2d ", chan);
    if (chan == edit_track.channel)
      wattroff(win, A_REVERSE);

    if (active_field == TEAF_Chan && chan == edit_track.channel) {
      wmove(win, row, 40);
      waddstr(win, "->");
    }
    ++row;
  }
}

void TrackEditor::draw_input_fields() {
  input_field("Bank MSB", edit_track.bank_msb, 4, active_field == TEAF_MSB);
  input_field("Bank LSB", edit_track.bank_lsb, 6, active_field == TEAF_LSB);
  input_field(" Program", edit_track.prog, 8, active_field == TEAF_Prog);
}

void TrackEditor::input_field(const char * const field_name, int val, int row, bool cursor_here) {
  wmove(win, row, 51);
  waddstr(win, field_name);
  waddstr(win, ": ");
  wattron(win, A_REVERSE);
  if (val == UNDEFINED)
    waddstr(win, "     ");
  else
    wprintw(win, " %3d ", val);
  wattroff(win, A_REVERSE);

  if (cursor_here) {
    wmove(win, row, 48);
    waddstr(win, "->");
    // FIXME blinking cursor
    // wmove(win, row, 51 + strlen(field_name) + 6);
    // wattron(win, A_BLINK);
    // waddch(win, ' ');
    // wattroff(win, A_BLINK);
  }
}

void TrackEditor::save() {
  track.name = edit_track.name;
  track.output = edit_track.output;
  track.channel = edit_track.channel;
  track.bank_msb = edit_track.bank_msb;
  track.bank_lsb = edit_track.bank_lsb;
  track.prog = edit_track.prog;
  track.one_shot = edit_track.one_shot;
}

void TrackEditor::edit_number(int ch) {
  int *ip = nullptr;

  switch (active_field) {
  case TEAF_MSB:
    ip = &edit_track.bank_msb;
    break;
  case TEAF_LSB:
    ip = &edit_track.bank_lsb;
    break;
  case TEAF_Prog:
    ip = &edit_track.prog;
    break;
  default:
    return;
  }

  if (ch == DELETE_KEY) {
    if (*ip >= 10)
      *ip /= 10;
    else if (*ip > 0)
      *ip = UNDEFINED;
    return;
  }

  int val = ch - '0';
  if (*ip == UNDEFINED)
    *ip = 0;
  *ip = *ip * 10 + val;
  if (*ip > 127)
    *ip = 127;
}

void TrackEditor::edit_name(int ch) {
  if (ch == 127) {
    if (!edit_track.name.empty())
      edit_track.name.pop_back();
  }
  else
    edit_track.name += ch;
}
