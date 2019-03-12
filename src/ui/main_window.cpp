#include "main_window.h"

MainWindow::MainWindow(BaseObjectType* window, const Glib::RefPtr<Gtk::Builder> &gladeRef) : Gtk::ApplicationWindow(window), m_builder(gladeRef) {
    Gtk::GLArea *glArea;

    // Set callback handlers
    m_builder->get_widget("importMediaBtn", m_importMediaBtn);
    m_builder->get_widget_derived("display_area", m_contextManager);

    Gtk::MenuItem *m_quitBtn;
    m_builder->get_widget("quitBtn", m_quitBtn);
    m_quitBtn->signal_activate().connect(sigc::ptr_fun(Gtk::Main::quit));
    m_importMediaBtn->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_import_media));
    
}

void MainWindow::on_import_media() {
  Gtk::FileChooserDialog dialog("Please choose a media file",
  Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

  //Add response buttons the the dialog:
  dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog.add_button("_Import", Gtk::RESPONSE_OK);

  int result = dialog.run();
  
  //TODO: Handle media file import
}

MainWindow::~MainWindow() {}
