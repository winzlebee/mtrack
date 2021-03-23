#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>
#include <memory>

#include "../render/context_manager.h"
#include "../playback/playback_manager.h"
#include "../playback/playback_source.h"
#include "../models/projectitem_model.h"
#include "timeline_widget.h"

class Project;

class MainWindow : public Gtk::ApplicationWindow {
    public:
        MainWindow(BaseObjectType* window, const Glib::RefPtr<Gtk::Builder> &gladeRef);
        virtual ~MainWindow();
    private:
        void setup_drag_drop();
        int get_media_from_path(const Gtk::TreeModel::Path& path);
        void on_media_drag_drop(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time);
    protected:
        // Signal Handlers for the main window menu
        void on_import_media();
        void on_media_icon_select(const Gtk::TreeModel::Path& path);
        void on_project_properties();
        void on_about();

		// Playback signal handlers
		void on_next_frame();
		void on_prev_frame();
		void on_first_frame();
		void on_last_frame();
		void on_play();
		void on_stop();
		void on_playback_source_change(bool loaded);
        void on_playback_status_changed(bool playing);
        
        Glib::RefPtr<Gtk::Builder> m_builder;

        std::unique_ptr<Project> m_project;
        
        // ContextManager handles the link between the UI and drawing to the screen
        // ContextManager class will be destructed automatically when the UI is destroyed
        ContextManager *m_contextManager;
        TimelineWidget *m_timeline;
		std::unique_ptr<PlaybackManager> m_playbackManager;
		std::unique_ptr<PlaybackSource> m_playbackSource;

		// Toolbar elements
		Gtk::ToolButton *m_nextFrameBtn;
		Gtk::ToolButton *m_prevFrameBtn;
		Gtk::ToolButton *m_firstFrameBtn;
		Gtk::ToolButton *m_lastFrameBtn;
		Gtk::ToolButton *m_playBtn;
		Gtk::ToolButton *m_stopBtn;
		Gtk::Widget *m_playbackWidget;

        // Memory for these is handled by the builder class
        Gtk::MenuItem *m_importMediaBtn;
        Gtk::MenuItem *m_projectPropertiesBtn;
        Gtk::MenuItem *m_aboutBtn;

        Gtk::IconView *m_mediaItems;

        // Model that the media item browser uses
        ProjectItemModel m_mediaModel;
        Glib::RefPtr<Gtk::ListStore> m_mediaListStore;
};

#endif
