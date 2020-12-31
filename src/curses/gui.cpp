#include <sstream>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "gui.h"
#include "../consts.h"
#include "../strange_loops.h"
#include "geometry.h"
#include "prompt_window.h"
#include "help_window.h"


static GUI *g_instance = 0;

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
  string name_regex;

  while (!done) {
    refresh_all();
    ch = getch();
    switch (ch) {
    case 'h': case '?':
      help();
      break;
    case '\e':                  /* escape */
      show_message("Sending panic...");
      sloops->panic(prev_cmd == '\e');
      show_message("Panic sent");
      clear_message_after(5);
      break;
    case 'q':
      done = TRUE;
      break;
    case KEY_RESIZE:
      resize_windows();
      break;
    }
    prev_cmd = ch;

    // TODO messages and code keys
    /* msg_name = @sloops->message_bindings[ch]; */
    /* @sloops->send_message(msg_name) if msg_name; */
    /* code_key = @sloops->code_bindings[ch]; */
    /* code_key.call if code_key; */
  }
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
  draw_tracks();
  message->draw();
  wnoutrefresh(stdscr);
  doupdate();
}

void GUI::set_window_data() {
}

void GUI::draw_tracks() {
  for (int i = 0; i < 16; ++i) {
    Track &track = sloops->current_scene().tracks[i];
    int track_num = i + 1;
    int col = i * 10 + 2;
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

    color_set(color_pair, nullptr);

    move(1, col);
    addstr("        ");

    move(2, col);
    addstr("   ");

    if (track_num < 10)
      addch(' ');
    else {
      attron(A_BOLD);
      addch('0' + (track_num / 10));
    }
    attron(A_BOLD);
    addch('0' + track_num % 10);
    attroff(A_BOLD);

    addstr("   ");

    move(3, col);
    addstr("        ");
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
