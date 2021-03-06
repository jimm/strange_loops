#ifndef GUI_H
#define GUI_H

using namespace std;

class Window;
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
  StrangeLoops *sloops;
  Window *message;

  int clear_msg_secs;
  int clear_msg_id;

  void event_loop();

  void edit_track(int track_num);

  void config_curses();
  void init_colors();
  void create_windows();
  void resize_windows();
  void free_windows();
  void refresh_all();
  void set_window_data();
  void draw_tracks();
  void close_screen();
  void help();
};

GUI *gui_instance();

#endif /* GUI_H */
