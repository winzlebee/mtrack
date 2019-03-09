#include <gtkmm.h>

class ContextManager {
    public:
        ContextManager(Gtk::GLArea*);
    private:
        void gl_init();
        void gl_destroy();
        bool gl_render(const Glib::RefPtr<Gdk::GLContext>&);
        Gtk::GLArea* gla;
};
