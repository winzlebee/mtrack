#include <memory>
#include <string>
#include <gtkmm.h>

#ifndef project_item_h
#define project_item_h

#include "../render/cmanager.h"

class ProjectItem {
    public:
        ProjectItem(std::string name);
		virtual ~ProjectItem() = 0;

		// Getters
		virtual bool isLoaded() = 0;
		virtual const unsigned int getTexId() = 0;
		virtual int get_num_frames() = 0;
		virtual double get_frame_rate() = 0;
		virtual double get_current_time_ms() = 0;
		virtual int get_current_frame() = 0;
		virtual Glib::RefPtr<Gdk::Pixbuf> get_pixbuf() = 0;

		// Manipulators
		virtual bool set_next_frame(int frame) = 0;
		virtual bool load_next_frame(ContextManager *context) = 0;
        virtual void load_media(std::string file_name, ContextManager *context) {};
        std::string getName();
    private:
        std::string m_name;
};

class VideoItem : public ProjectItem {
    public:
        VideoItem(std::string name) : ProjectItem(name) {};
		~VideoItem() override;
        
		// Video utility functions for use with other contexts
		bool isLoaded();
		const unsigned int getTexId();
		int get_num_frames();
		double get_frame_rate();
		double get_current_time_ms();
		int get_current_frame();
		Glib::RefPtr<Gdk::Pixbuf> get_pixbuf();

		// Set the frame to be loaded next time load_next_frame is called. Should be immediately preceded by load_next_frame.
		bool set_next_frame(int frame);

		// Advance to the next frame in the sequence. False if no more frames. Always happens within a particular context.
		bool load_next_frame(ContextManager *context);

		// Load the texture corresponding to the media into the specified context
		void load_media(std::string file_name, ContextManager *context);

    private:
        std::unique_ptr<cv::VideoCapture> video;
		Glib::RefPtr<Gdk::Pixbuf> m_pixelBuffer;
        bool loaded = false;
        unsigned int texture_id = 0;
};

#endif
