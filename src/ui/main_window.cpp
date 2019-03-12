#include "main_window.h"
#include "../project/project.h"

MainWindow::MainWindow(BaseObjectType* window, const Glib::RefPtr<Gtk::Builder> &gladeRef) : Gtk::ApplicationWindow(window), m_builder(gladeRef) {
  Gtk::GLArea *glArea;

  // We've started the main window - create some new project properties
  m_project = std::make_unique<Project>();

  // Set callback handlers
  m_builder->get_widget("importMediaBtn", m_importMediaBtn);
  m_builder->get_widget("projectPropertiesBtn", m_projectPropertiesBtn);
  m_builder->get_widget("aboutBtn", m_aboutBtn);
  m_builder->get_widget_derived("display_area", m_contextManager, m_project.get());

  Gtk::MenuItem *m_quitBtn;
  m_builder->get_widget("quitBtn", m_quitBtn);

  // Signal handlers for menu buttons
  m_quitBtn->signal_activate().connect(sigc::ptr_fun(Gtk::Main::quit));
  m_importMediaBtn->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_import_media));
  m_projectPropertiesBtn->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_project_properties));
  m_aboutBtn->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_about));
  
}

void MainWindow::on_about() {
  Gtk::Dialog *aboutDialog;

  m_builder->get_widget("aboutDialog", aboutDialog);
  aboutDialog->run();
}

void MainWindow::on_project_properties() {
  Gtk::Dialog *propDialog;

  // Run the project properties dialog
  m_builder->get_widget("projectPropertiesDialog", propDialog);

  Gtk::SpinButton *hres;
  Gtk::SpinButton *vres;
  Gtk::SpinButton *fps;
  // Load all of the fields from the dialog
  m_builder->get_widget("projectHResolution", hres);
  m_builder->get_widget("projectVResolution", vres);
  m_builder->get_widget("projectFPS", fps);
  
  hres->set_value(m_project->getSettings().hResolution);
  vres->set_value(m_project->getSettings().vResolution);
  fps->set_value(m_project->getSettings().fRate);


  int result = propDialog->run();
  if (result == Gtk::RESPONSE_APPLY) {
    // Apply the changed project settings
    m_project->getSettings().hResolution = hres->get_value();
    m_project->getSettings().vResolution = vres->get_value();
    m_project->getSettings().fRate = fps->get_value();
  }
  propDialog->hide();
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
