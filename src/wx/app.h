#ifndef WX_APP_H
#define WX_APP_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
 #include <wx/wx.h>
#endif

class Frame;

class App: public wxApp {
public:
  App();
  ~App();

  void show_user_message(std::string msg);
  void show_user_message(std::string msg, int clear_secs);

private:
  Frame *frame;
  wxString command_line_path;

  virtual bool OnInit();
  virtual void OnInitCmdLine(wxCmdLineParser &);
  virtual bool OnCmdLineParsed(wxCmdLineParser &);
  virtual int OnExit();

  void list_all_devices();
};

App *app_instance();

#endif /* WX_APP_H */
