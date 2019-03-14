#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>
#include <memory>

#include "../render/cmanager.h"
#include "../models/projectitem_model.h"

class Project;

class MainWindow : public Gtk::ApplicationWindow {
    public:
        MainWindow(BaseObjectType* window, const Glib::RefPtr<Gtk::Builder> &gladeRef);
        virtual ~MainWindow();
    protected:
        // Signal Handlers for the main window menu
        void on_import_media();
        void on_project_properties();
        void on_about();

		// Playback signal handlers
		void on_next_frame();
        
        Glib::RefPtr<Gtk::Builder> m_builder;

        std::unique_ptr<Project> m_project;
        
        // ContextManager handles the link between the UI and drawing to the screen
        // ContextManager class will be destructed automatically when the UI is destroyed
        ContextManager *m_contextManager;

		// Toolbar elements
		Gtk::ToolButton *m_nextFrameBtn;

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
