#include <gtkmm.h>
#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "ui/main_window.h"

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "com.wizzledonker.mtrack");
	  
	// Load the main GLADE ui
	auto builder = Gtk::Builder::create();

	try {
		builder->add_from_file("main_window.glade");
	}
	catch(const Glib::FileError& ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
    }
	catch(const Gtk::BuilderError& ex) {
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return 1;
	}

    MainWindow* window;
    
	builder->get_widget_derived("main_window", window);

	int result = app->run(*window);
	
	delete window;
	return result;
}
