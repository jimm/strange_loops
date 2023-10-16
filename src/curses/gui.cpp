#include <sstream>
#include <map>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "gui.h"
#include "../consts.h"
#include "../strange_loops.h"
#include "geometry.h"
#include "track_editor.h"
#include "prompt_window.h"
#include "help_window.h"


static GUI *g_instance = 0;
static map<int, int> char_to_track_num {
  // play
  {'1', 0}, {'2', 1}, {'3', 2}, {'4', 3}, {'5', 4}, {'6', 5}, {'7', 6}, {'8', 7},
  {'a', 8}, {'s', 9}, {'d', 10}, {'f', 11}, {'g', 12}, {'h', 13}, {'j', 14}, {'k', 15},
  // stop
  {'q', 0}, {'w', 1}, {'e', 2}, {'r', 3}, {'t', 4}, {'y', 5}, {'u', 6}, {'i', 7},
  {'z', 8}, {'x', 9}, {'c', 10}, {'v', 11}, {'b', 12}, {'n', 13}, {'m', 14}, {',', 15}
};
static int track_chars[16] = {
  '1', '2', '3', '4', '5', '6', '7', '8',
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k'
};

enum TrackStateColorPair {
  TC_Empty = 1,
  TC_Recording,
  TC_Overdubbing,
  TC_Playing,
  TC_Stopped
};

GUI *gui_instance() {
  return g_instance;
}

GUI::GUI(StrangeLoops *sl)
  : sloops(sl), clear_msg_id(0)
{
  g_instance = this;
}

GUI::~GUI() {
  if (g_instance == this)
    g_instance = 0;
}

void GUI::run() {
  config_curses();
  init_colors();
  create_windows();
  event_loop();
  clear();
  refresh();
  close_screen();
  free_windows();
}

void GUI::event_loop() {
  bool done = FALSE;
  int ch, prev_cmd = 0;
  PromptWindow *pwin;

  while (!done) {
    refresh_all();
    ch = getch();
    switch (ch) {
    case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
    case 'a': case 's': case 'd': case 'f': case 'g': case 'h': case 'j': case 'k':
      sloops->current_scene().take_action(char_to_track_num[ch], TrackRecordOverdubPlay);
      break;
    case 'q': case 'w': case 'e': case 'r': case 't': case 'y': case 'u': case 'i':
    case 'z': case 'x': case 'c': case 'v': case 'b': case 'n': case 'm': case ',':
      sloops->current_scene().take_action(char_to_track_num[ch], TrackStopClear);
      break;
    case '`':
      ch = getch();
      try {
        edit_track(char_to_track_num.at(ch));
      }
      catch (out_of_range) {
        ;
      }
      break;
    case '?':
      help();
      break;
    case '\\':
      show_message("Sending panic...");
      sloops->panic(prev_cmd == '\e');
      show_message("Panic sent");
      clear_message_after(5);
      break;
    case '\e':
      sloops->current_scene().take_action(0, AllStartStop);
      break;
    case 'Q':
      done = true;
      break;
    case KEY_RESIZE:
      resize_windows();
      break;
    }
    prev_cmd = ch;
  }
}

void GUI::edit_track(int track_num) {
  if (track_num < 0 || track_num >= NUM_TRACKS)
    return;

  char buf[BUFSIZ];

  snprintf(buf, BUFSIZ, "Track Editor: %c", track_chars[track_num]);
  TrackEditor *ed = new TrackEditor(
    geom_track_editor_rect(), buf,
    sloops->outputs(), sloops->current_scene().track(track_num)
  );
  ed->run();
  delete ed;
}

void GUI::config_curses() {
  initscr();
  start_color();
  cbreak();                     /* unbuffered input */
  noecho();                     /* do not show typed keys */
  keypad(stdscr, true);         /* enable arrow keys and window resize as keys */
  nl();                         /* return key => newline, \n => \r\n */
  curs_set(0);                  /* cursor: 0 = invisible, 1 = normal */
}

void GUI::init_colors() {
  init_pair(TC_Empty, COLOR_WHITE, COLOR_BLACK);
  init_pair(TC_Recording, COLOR_WHITE, COLOR_RED);
  init_pair(TC_Overdubbing, COLOR_WHITE, COLOR_CYAN);
  init_pair(TC_Playing, COLOR_WHITE, COLOR_GREEN);
  init_pair(TC_Stopped, COLOR_WHITE, COLOR_BLUE);
}

void GUI::create_windows() {
  message = new Window(geom_message_rect(), "");

  scrollok(stdscr, false);
  scrollok(message->win, false);
}

void GUI::resize_windows() {
  message->move_and_resize(geom_message_rect());
}

void GUI::free_windows() {
  delete message;
}

void GUI::refresh_all() {
  set_window_data();
  clear();
  draw_tracks();
  message->draw();
  wnoutrefresh(stdscr);
  doupdate();
}

void GUI::set_window_data() {
}

void GUI::draw_tracks() {
  for (int i = 0; i < 16; ++i) {
    Track &track = sloops->current_scene().track(i);
    int track_num = i + 1;
    int row = i < 8 ? 1 : 6;
    int col = (i < 8 ? i : i - 8) * 9 + 2;
    TrackStateColorPair color_pair = TC_Empty;

    // TODO set track state color
    switch (track.state) {
    case Empty:
      color_pair = TC_Empty;
      break;
    case Recording:
      color_pair = TC_Recording;
      break;
    case Overdubbing:
      color_pair = TC_Overdubbing;
      break;
    case Playing:
      color_pair = TC_Playing;
      break;
    case Stopped:
      color_pair = TC_Stopped;
      break;
    }

    // Color pair 0 doesn't do the same thing on dark- and light-themed
    // terminals, so we use reverse mode to draw it instead.
    if (color_pair == TC_Empty)
      attron(A_REVERSE);
    else
      color_set(color_pair, nullptr);

    move(row++, col);
    addstr("       ");

    move(row++, col);
    addstr("   ");
    attron(A_BOLD);
    addch(track_chars[i]);
    attroff(A_BOLD);
    addstr("   ");

    move(row++, col);
    addstr("       ");

    if (color_pair == TC_Empty)
      attroff(A_REVERSE);
    else
      color_set(0, nullptr);

    move(row, col);
    std::string truncated_name = track.name.substr(0, 7);
    addstr(truncated_name.c_str());
  }
  use_default_colors();
}

void GUI::close_screen() {
  curs_set(1);
  echo();
  nl();
  noraw();
  nocbreak();
  refresh();
  endwin();
}

void GUI::help() {
  rect r = geom_help_rect();
  HelpWindow hw(r, "Help");
  hw.draw();
  wnoutrefresh(hw.win);
  doupdate();
  getch();                      /* wait for key and eat it */
}

void GUI::show_message(string msg) {
  WINDOW *win = message->win;
  wclear(win);
  message->make_fit(msg, 0);
  waddstr(win, msg.c_str());
  wrefresh(win);
  doupdate();
}

void GUI::clear_message() {
  WINDOW *win = message->win;
  wclear(win);
  wrefresh(win);
  doupdate();
}

void *clear_message_thread(void *gui_vptr) {
  GUI *gui = (GUI *)gui_vptr;
  int clear_message_id = gui->clear_message_id();

  sleep(gui->clear_message_seconds());

  // Only clear the window if the id hasn't changed
  if (gui->clear_message_id() == clear_message_id)
    gui->clear_message();
  return nullptr;
}

void GUI::clear_message_after(int secs) {
  clear_msg_secs = secs;
  clear_msg_id++;

  pthread_t pthread;
  pthread_create(&pthread, 0, clear_message_thread, this);
}
