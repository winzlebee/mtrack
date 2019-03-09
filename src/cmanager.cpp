#include <gtkmm.h>
#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "cmanager.h"

ContextManager::ContextManager(Gtk::GLArea *glArea) {
    gla = glArea;
    
    // Setup the basic signals for a context manager's management
    gla->signal_realize().connect(sigc::mem_fun(*this, &ContextManager::gl_init));
    gla->signal_render().connect(sigc::mem_fun(*this, &ContextManager::gl_render));
    gla->signal_unrealize().connect(sigc::mem_fun(*this, &ContextManager::gl_destroy));
}

void ContextManager::gl_init() {
	std::cout << "OpenGL window is being initialized..." << std::endl;
	gla->make_current();
	  try
	  {
		gla->throw_if_error();
		// TODO: Initialize shaders, etc
	  }
	  catch(const Gdk::GLError& gle)
	  {
		std::cerr << "An error occured making the context current during realize:" << std::endl;
		std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
	}
}

bool ContextManager::gl_render(const Glib::RefPtr<Gdk::GLContext>& /* context */) {
	try {
		gla->throw_if_error();
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Draw here

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
	gla->make_current();
	  try
	  {
		gla->throw_if_error();
		// TODO: Remove all created stuff during OpenGL realize
	  }
	  catch(const Gdk::GLError& gle)
	  {
		std::cerr << "An error occured making the context current during unrealize:" << std::endl;
		std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
	}
}
