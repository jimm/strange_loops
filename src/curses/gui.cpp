#include <sstream>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "gui.h"
#include "../consts.h"
#include "../strange_loops.h"
#include "geometry.h"
#include "tracks_window.h"
#include "prompt_window.h"
#include "help_window.h"


static GUI *g_instance = 0;

GUI *gui_instance() {
  return g_instance;
}

GUI::GUI(StrangeLoops *sloops)
  : sl(sloops), clear_msg_id(0)
{
  g_instance = this;
}

GUI::~GUI() {
  if (g_instance == this)
    g_instance = 0;
}

void GUI::run() {
  config_curses();
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
      sl->panic(prev_cmd == '\e');
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
    /* msg_name = @sl->message_bindings[ch]; */
    /* @sl->send_message(msg_name) if msg_name; */
    /* code_key = @sl->code_bindings[ch]; */
    /* code_key.call if code_key; */
  }
}

void GUI::config_curses() {
  initscr();
  cbreak();                     /* unbuffered input */
  noecho();                     /* do not show typed keys */
  keypad(stdscr, true);         /* enable arrow keys and window resize as keys */
  nl();                         /* return key => newline, \n => \r\n */
  curs_set(0);                  /* cursor: 0 = invisible, 1 = normal */
}

void GUI::create_windows() {
  tracks_window = new TracksWindow(geom_tracks_rect(), "Tracks", sl->current_scene().tracks);
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
  tracks_window->draw();
  message->draw();
  wnoutrefresh(stdscr);
  doupdate();
}

void GUI::set_window_data() {
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
