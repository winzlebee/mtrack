#include <gtkmm.h>
#include <vector>

#ifndef CMANAGER_H
#define CMANAGER_H

#include "../project/project_item.h"

class Project;

class ContextManager : public Gtk::GLArea {
    public:
        ContextManager(BaseObjectType*, Glib::RefPtr<Gtk::Builder> &gladeRef, Project *project);
        virtual ~ContextManager();

        // Force a redisplay when something about the context display changes
        void redisplay();
        bool render_media(const ProjectItem *item, int frame);
    private:

        // Class variables
        Project *m_project;
        std::vector<float> m_coords;

        void generate_coords();

        void gl_init();
        void gl_destroy();
        bool gl_render(const Glib::RefPtr<Gdk::GLContext>&);

        // Override the on_resize handler to allow for redrawing of the video display.
        void on_resize(int width, int height);
        
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