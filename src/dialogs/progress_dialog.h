#pragma once

#include <gtkmm.h>

class ProgressDialog : public Gtk::Dialog
{
public:
  ProgressDialog(const std::string &label);
  ~ProgressDialog();

  Glib::RefPtr<Gio::Cancellable> cancellable() const
  {
    return m_cancellable;
  }

private:
  Gtk::ProgressBar m_progressBar;

  Glib::RefPtr<Gio::Cancellable> m_cancellable;
};
