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
        
        Glib::RefPtr<Gtk::Builder> m_builder;
        
        // Memory for these pointers is handled by the builder class
        std::unique_ptr<ContextManager> m_contextManager;
        Gtk::MenuItem *m_importMediaBtn;
};

#endif
