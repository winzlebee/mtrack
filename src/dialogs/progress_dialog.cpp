#include "progress_dialog.h"

#include <glibmm/i18n.h>

ProgressDialog::ProgressDialog(const std::string &title)
  : Gtk::Dialog(title, false)
  , m_cancellable(Gio::Cancellable::create())
{
  get_content_area()->add(m_progressBar);
  auto *cancel_btn = add_button(_("_Cancel"), Gtk::RESPONSE_CANCEL);

  auto bound = sigc::mem_fun(*m_cancellable.get(), &Gio::Cancellable::cancel);
  cancel_btn->signal_clicked().connect(bound);
}

ProgressDialog::~ProgressDialog()
{
}
