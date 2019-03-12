#include <memory>
#include <vector>

#include "project_item.h"

typedef std::vector<std::unique_ptr<ProjectItem>> MediaArray;

struct ProjectSettings {
    double hResolution = 1280.0;
    double vResolution = 720.0;
    double fRate = 25.0;
};

class Project {
    public:
        Project() {};
        Project(ProjectSettings sett) : settings(sett) {}
        
        ProjectSettings getSettings() { return settings; }
        bool importMedia(std::unique_ptr<ProjectItem> media);
        bool hasMedia();
        ProjectItem* getLoadedMedia();
    private:
        ProjectSettings settings;
        MediaArray projectMedia;
};
