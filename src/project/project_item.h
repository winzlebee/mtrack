#include <memory>

#ifndef project_item_h
#define project_item_h

namespace cv {
    class VideoCapture;
}

class ProjectItem {
    public:
        virtual bool isLoaded() = 0;
        virtual unsigned int getTexId() = 0;
};

class VideoItem : public ProjectItem {
    public:
        VideoItem(const char* fname);
        double getFps();
        double getLengthFrames();
        double getLengthSeconds();
        bool isLoaded();
        unsigned int getTexId();
    private:
        void load_video(const char* name);
        std::unique_ptr<cv::VideoCapture> video;
        bool loaded = false;
        unsigned int texture_id = 0;
};

#endif
