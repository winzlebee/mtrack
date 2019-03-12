#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>
#include <memory>

#include "../render/cmanager.h"

class MainWindow : public Gtk::ApplicationWindow {
    public:
        MainWindow(BaseObjectType* window, const Glib::RefPtr<Gtk::Builder> &gladeRef);
        virtual ~MainWindow();
    protected:
        // Signal Handlers for the main window
        void on_import_media();
        void on_project_properties();
        void on_about();
        
        Glib::RefPtr<Gtk::Builder> m_builder;
        
        // ContextManager handles the link between the UI and drawing to the screen
        // ContextManager class will be destructed automatically when the UI is destroyed
        ContextManager* m_contextManager;

        // Memory for these is handled by the builder class
        Gtk::MenuItem *m_importMediaBtn;
        Gtk::MenuItem *m_projectPropertiesBtn;
        Gtk::MenuItem *m_aboutBtn;
};

#endif
