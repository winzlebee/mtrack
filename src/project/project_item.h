#include <memory>

#ifndef video_h
#define video_h

namespace cv {
    class VideoCapture;
}

class ProjectItem {
    public:
        virtual bool isLoaded() = 0;
        virtual int getTexId() = 0;
        virtual void render() = 0; // Render to the current context. By default, fills the context.
};

class VideoItem : public ProjectItem {
    public:
        VideoItem(const char* fname);
        double getFps();
        double getLengthFrames();
        double getLengthSeconds();
        bool isLoaded();
        int getTexId();
        void render();
    private:
        void load_video(const char* name);
        void generate3DView();
        std::unique_ptr<cv::VideoCapture> video;
        bool loaded = false;
        unsigned int texture_id = 0;
        unsigned int vao_id = 0;
        unsigned int vbo_id = 0;
        unsigned int program_id = 0;
};

#endif
