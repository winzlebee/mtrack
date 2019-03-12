#include <gtkmm.h>

#ifndef CMANAGER_H
#define CMANAGER_H

class Project;

class ContextManager : public Gtk::GLArea {
    public:
        ContextManager(BaseObjectType*, Glib::RefPtr<Gtk::Builder> &gladeRef, Project *project);
        virtual ~ContextManager();
    private:

        // Class variables
        Project *m_project;

        void gl_init();
        void gl_destroy();
        bool gl_render(const Glib::RefPtr<Gdk::GLContext>&);
        
        void draw_video();
        
        void init_buffers();
        void init_shaders();
        
        unsigned int create_shader();
        
        // Variables related to rendering. Later, these should be separated out.
        unsigned int vaoId;
        unsigned int vboId;
        
        unsigned int programId;
};

#endif