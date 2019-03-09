#include <gtkmm.h>
#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "cmanager.h"

Gtk::Window* window;
Gtk::GLArea* glArea;

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "com.wizzledonker.mtrack");
	  
	// Load the main GLADE ui
	auto builder = Gtk::Builder::create();

	try {
		builder->add_from_file("res/main_window.glade");
	}
	catch(const Glib::FileError& ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
	}

	builder->get_widget("main_window", window);
	builder->get_widget("display_area", glArea);
	
	// ContextManager manages everything to do with the context. It hooks the signals and makes sure the OpenGL stuff is handled well.
	ContextManager context(glArea);

	int result = app->run(*window);
	delete window;
	return result;
}
