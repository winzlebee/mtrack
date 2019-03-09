#include <gtkmm.h>
#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

Gtk::Window* window;
Gtk::GLArea* glArea;

void opengl_init() {
	std::cout << "OpenGL window is being initialized..." << std::endl;
	glArea->make_current();
	  try
	  {
		glArea->throw_if_error();
		// TODO: Initialize shaders, etc
	  }
	  catch(const Gdk::GLError& gle)
	  {
		std::cerr << "An error occured making the context current during realize:" << std::endl;
		std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
	}
}

bool opengl_render(const Glib::RefPtr<Gdk::GLContext>& /* context */) {
	try {
		glArea->throw_if_error();
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

void opengl_destroy() {
	std::cout << "Destroying OpenGL window..." << std::endl;
	glArea->make_current();
	  try
	  {
		glArea->throw_if_error();
		// TODO: Remove all created stuff during OpenGL realize
	  }
	  catch(const Gdk::GLError& gle)
	  {
		std::cerr << "An error occured making the context current during unrealize:" << std::endl;
		std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
	}
}

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
	
	// Connect a callback for when the OpenGL area is updated
	glArea->signal_realize().connect(sigc::ptr_fun(&opengl_init));
	glArea->signal_render().connect(sigc::ptr_fun(&opengl_render));
	glArea->signal_unrealize().connect(sigc::ptr_fun(&opengl_destroy));

	int result = app->run(*window);
	delete window;
	return result;
}
