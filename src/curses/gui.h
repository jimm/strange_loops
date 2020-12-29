#ifndef GUI_H
#define GUI_H

using namespace std;

class Window;
class TracksWindow;
class StrangeLoops;

class GUI {
public:
  GUI(StrangeLoops *sl);
  ~GUI();

  void run();

  void show_message(string);
  void clear_message();
  void clear_message_after(int);
  int clear_message_seconds() { return clear_msg_secs; }
  int clear_message_id() { return clear_msg_id; }

private:
  StrangeLoops *sl;

  Window *message;
  TracksWindow *tracks_window;

  int clear_msg_secs;
  int clear_msg_id;

  void event_loop();
  void config_curses();
  void create_windows();
  void resize_windows();
  void free_windows();
  void refresh_all();
  void set_window_data();
  void close_screen();
  void help();
};

GUI *gui_instance();

#endif /* GUI_H */
