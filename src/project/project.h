#include <memory>
#include <vector>

#include "video.h"

typedef std::vector<std::unique_ptr<MediaDisplay>> MediaArray;

struct ProjectSettings {
    double fRate = 24.0;
};

class Project {
    public:
        Project() {};
        Project(ProjectSettings sett) : settings(sett) {}
        
        ProjectSettings getSettings() { return settings; }
        bool importMedia(std::unique_ptr<MediaDisplay> media);
        bool hasMedia();
        MediaDisplay* getLoadedMedia();
    private:
        ProjectSettings settings;
        MediaArray projectMedia;
};
