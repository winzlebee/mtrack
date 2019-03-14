#include <memory>
#include <string>
#include <opencv2/opencv.hpp>

#ifndef project_item_h
#define project_item_h

#include "../render/cmanager.h"

class ProjectItem {
    public:
        ProjectItem(std::string name);
        virtual bool isLoaded() = 0;
        virtual const unsigned int getTexId() = 0;
		virtual bool advance_frame(ContextManager *context) = 0;
        virtual void load_media(std::string file_name, ContextManager *context) {};
        std::string getName();
    private:
        std::string m_name;
};

class VideoItem : public ProjectItem {
    public:
        VideoItem(std::string name) : ProjectItem(name) {};
        
		// Video utility functions for use with other contexts

		// Advance to the next frame in the sequence. False if no more frames. Always happens within a particular context.
		bool advance_frame(ContextManager *context);

        bool isLoaded();
        const unsigned int getTexId();
        void load_media(std::string file_name, ContextManager *context);
    private:
        std::unique_ptr<cv::VideoCapture> video;
        bool loaded = false;
        unsigned int texture_id = 0;
};

#endif
