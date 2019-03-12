#include <gtkmm.h>
#include <iostream>
#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "cmanager.h"
#include "../project/project.h"

// Explicitly define shader prefix and suffixes

const GLchar *vert_src ="\n" \
"#version 330                                  \n" \
"#extension GL_ARB_explicit_attrib_location: enable  \n" \
"                                              \n" \
"layout(location = 0) in vec2 in_position;     \n" \
"                                              \n" \
"void main()                                   \n" \
"{                                             \n" \
"  gl_Position = vec4(in_position, 0.0, 1.0);  \n" \
"}                                             \n";

const GLchar *frag_src ="\n" \
"void main (void)                              \n" \
"{                                             \n" \
"  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);    \n" \
"}                                             \n";

ContextManager::ContextManager(BaseObjectType *glArea, Glib::RefPtr<Gtk::Builder> &gladeRef, Project *project) : Gtk::GLArea(glArea), m_project(project) {
    
    // Generate coords based on the project settings and the width and heightof the window.
    generate_coords();

    // Setup the basic signals for a context manager's management
    gl_init();
    this->signal_render().connect(sigc::mem_fun(*this, &ContextManager::gl_render));
    this->signal_unrealize().connect(sigc::mem_fun(*this, &ContextManager::gl_destroy));
}

ContextManager::~ContextManager() {}

void ContextManager::generate_coords() {
  // For now, this draws a simple triangle. Always push the x straight to the edges
  float c_width = this->get_width();
  float c_height = this->get_height();

  float c_vaspect = (c_height/c_width);
  float c_haspect = (c_width/c_height);
  float vaspect = std::min((m_project->getSettings().vResolution/m_project->getSettings().hResolution)/c_vaspect, 1.0);
  float haspect = std::min((m_project->getSettings().hResolution/m_project->getSettings().vResolution)/c_haspect, 1.0);

  std::cout << vaspect << std::endl;

  m_coords = {
      -haspect, -vaspect,
      -haspect, vaspect,
      haspect, vaspect,
      haspect, vaspect,
      haspect, -vaspect,
      -haspect, -vaspect
  };
}

void ContextManager::redisplay() {
  this->make_current();
  // Avoid reallocating the buffer store by using glBufferSubData
  generate_coords();
  glBindVertexArray(vaoId);
  glBindBuffer(GL_ARRAY_BUFFER, vboId);
  glBufferSubData(GL_ARRAY_BUFFER, 0, m_coords.size()*sizeof(GLfloat), &m_coords.front());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void ContextManager::on_resize(int width, int height) {
  glViewport(0, 0, width, height);
  redisplay();
}

void ContextManager::init_buffers() {
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, m_coords.size() * sizeof(GLfloat), &m_coords.front(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDisableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ContextManager::init_shaders() {
    // At the moment, we don't do any error checking. This will definitely be necessary when we start
    // Accepting user input for the shaders.
    
    GLuint frag_shader, vert_shader;
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    vert_shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(frag_shader, 1, &frag_src, NULL);
    glShaderSource(vert_shader, 1, &vert_src, NULL);

    glCompileShader(frag_shader);
    glCompileShader(vert_shader);

    programId = glCreateProgram();
    glAttachShader(programId, frag_shader);
    glAttachShader(programId, vert_shader);
    glLinkProgram(programId);
}

void ContextManager::gl_init() {
	std::cout << "OpenGL window is being initialized..." << std::endl;
	this->make_current();
	  try
	  {
		this->throw_if_error();
		// TODO: Initialize shaders, etc
		init_shaders();
		init_buffers();
	  }
	  catch(const Gdk::GLError& gle)
	  {
		std::cerr << "An error occurzed making the context current during realize:" << std::endl;
		std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
	}
}

void ContextManager::draw_video() {
    glUseProgram(programId);

    glBindVertexArray(vaoId);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glUseProgram(0);
}

bool ContextManager::gl_render(const Glib::RefPtr<Gdk::GLContext>& /* context */) {
    //gla->make_current();
	try {
	
		this->throw_if_error();
		glClearColor(m_project->getSettings().color.get_red(),
                 m_project->getSettings().color.get_green(),
                 m_project->getSettings().color.get_blue(),
                 m_project->getSettings().color.get_alpha());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Draw here
		draw_video();

		glFlush();

		return true;
	}
	catch(const Gdk::GLError& gle) {
		std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
		std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
		return false;
	}
}

void ContextManager::gl_destroy() {
	std::cout << "Destroying OpenGL window..." << std::endl;
	this->make_current();
	  try
	  {
		this->throw_if_error();
		// TODO: Remove all created stuff during OpenGL realize
	    glDeleteBuffers(1, &vaoId);
        glDeleteProgram(programId);
	  }
	  catch(const Gdk::GLError& gle)
	  {
		std::cerr << "An error occured making the context current during unrealize:" << std::endl;
		std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
	}
}
