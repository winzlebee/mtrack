#include <gtkmm.h>
#include <iostream>

Gtk::Window* window;

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

	return app->run(*window);
}
