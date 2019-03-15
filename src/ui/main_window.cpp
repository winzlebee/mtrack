#include "main_window.h"
#include "../project/project.h"
#include "../models/projectitem_model.h"

MainWindow::MainWindow(BaseObjectType* window, const Glib::RefPtr<Gtk::Builder> &gladeRef) : Gtk::ApplicationWindow(window), m_builder(gladeRef) {
  Gtk::GLArea *glArea;

  // We've started the main window - create some new project properties
  m_project = std::make_unique<Project>();
  m_playbackManager = std::make_unique<PlaybackManager>();

  m_playbackManager->signal_source_changed().connect(sigc::mem_fun(*this, &MainWindow::on_playback_source_change));

  // Set callback handlers
  m_builder->get_widget("importMediaBtn", m_importMediaBtn);
  m_builder->get_widget("projectPropertiesBtn", m_projectPropertiesBtn);
  m_builder->get_widget("aboutBtn", m_aboutBtn);
  m_builder->get_widget_derived("display_area", m_contextManager, m_project.get());
  m_builder->get_widget("mediaIconView", m_mediaItems);

  // Playback widget
  m_builder->get_widget("nextFrameBtn", m_nextFrameBtn);
  m_builder->get_widget("prevFrameBtn", m_prevFrameBtn);
  m_builder->get_widget("lastFrameBtn", m_lastFrameBtn);
  m_builder->get_widget("firstFrameBtn", m_firstFrameBtn);
  m_builder->get_widget("playBtn", m_playBtn);
  m_builder->get_widget("stopBtn", m_stopBtn);
  m_builder->get_widget("playbackWidget", m_playbackWidget);

  // Set up models
  m_mediaListStore = Gtk::ListStore::create(m_mediaModel);
  m_mediaItems->set_model(m_mediaListStore);
  m_mediaItems->set_text_column(0);

  Gtk::MenuItem *m_quitBtn;
  m_builder->get_widget("quitBtn", m_quitBtn);

  // Signal handlers for menu buttons
  m_quitBtn->signal_activate().connect(sigc::ptr_fun(Gtk::Main::quit));
  m_importMediaBtn->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_import_media));
  m_projectPropertiesBtn->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_project_properties));
  m_aboutBtn->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_about));

  m_nextFrameBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_next_frame));
  m_prevFrameBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_prev_frame));
  m_firstFrameBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_first_frame));
  m_lastFrameBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_last_frame));
  m_playBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_play));
  m_stopBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_stop));
  
}

// Playback widget
void MainWindow::on_next_frame() {
	m_playbackManager->next();
}

void MainWindow::on_prev_frame() {
	m_playbackManager->prev();
}

void MainWindow::on_first_frame() {
	m_playbackManager->first();
}

void MainWindow::on_last_frame() {
	m_playbackManager->last();
}

void MainWindow::on_play() {
	if (!m_playbackManager->isPlaying()) {
		m_playbackManager->play();
		m_playBtn->set_stock_id(Gtk::Stock::MEDIA_PAUSE);
	}
	else {
		m_playbackManager->pause();
		m_playBtn->set_stock_id(Gtk::Stock::MEDIA_PLAY);
	}
}

void MainWindow::on_stop() {
	m_playbackManager->stop();
}

void MainWindow::on_playback_source_change(bool loaded) {
	m_playbackWidget->set_sensitive(loaded);
}

void MainWindow::on_about() {
  Gtk::Dialog *aboutDialog;

  m_builder->get_widget("aboutDialog", aboutDialog);
  int response = aboutDialog->run();

  if (response != GTK_RESPONSE_NONE) aboutDialog->hide();
}

void MainWindow::on_project_properties() {
  Gtk::Dialog *propDialog;

  // Run the project properties dialog
  m_builder->get_widget("projectPropertiesDialog", propDialog);

  Gtk::SpinButton *hres;
  Gtk::SpinButton *vres;
  Gtk::SpinButton *fps;
  Gtk::ColorButton *color;

  // Load all of the fields from the dialog
  m_builder->get_widget("projectHResolution", hres);
  m_builder->get_widget("projectVResolution", vres);
  m_builder->get_widget("projectFPS", fps);
  m_builder->get_widget("projectBgColor", color);
  
  hres->set_value(m_project->getSettings().hResolution);
  vres->set_value(m_project->getSettings().vResolution);
  fps->set_value(m_project->getSettings().fRate);
  color->set_rgba(m_project->getSettings().color);

  int result = propDialog->run();
  if (result == Gtk::RESPONSE_APPLY) {
    // Apply the changed project settings
    m_project->getSettings().hResolution = hres->get_value();
    m_project->getSettings().vResolution = vres->get_value();
    m_project->getSettings().fRate = fps->get_value();
    m_project->getSettings().color = color->get_rgba();

    // Update the resolution in view
    m_contextManager->redisplay();
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
  
  if (result == Gtk::RESPONSE_OK) {
    // Create a new ProjectItem and load the media assosciated with it
    std::unique_ptr<ProjectItem> item = std::make_unique<VideoItem>(dialog.get_file()->get_basename());

    // Load the texture assosciated with the media file into the context manager
    item->load_media(dialog.get_filename(), m_contextManager);

    // Render the first frame of the item using the contextmanager. TODO: Create custom playback widget and set there
    if (item->isLoaded()) {
      Gtk::TreeModel::iterator modelit = m_mediaListStore->append();
      Gtk::TreeModel::Row newRow = *modelit;
      newRow[m_mediaModel.m_col_name] = item->getName();

      m_contextManager->render_media(item.get());

	  m_playbackManager->clearSource();
	  m_playbackSource = std::make_unique<MediaPlaybackSource>(item.get(), m_contextManager);
	  m_playbackManager->setSource(m_playbackSource.get());

      m_project->importMedia(std::move(item));
    } else {
      Gtk::MessageDialog unableToOpenMsg(*this, "Invalid media file");
      unableToOpenMsg.set_secondary_text(dialog.get_file()->get_basename() + " is not a valid media file.");
      unableToOpenMsg.run();
    }
  }
}

MainWindow::~MainWindow() {}
