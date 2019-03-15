#include <memory>
#include <vector>
#include <gtkmm.h>

#include "project_item.h"

typedef std::vector<std::unique_ptr<ProjectItem>> MediaArray;

struct ProjectSettings {
    ProjectSettings() {
        color.set_rgba(0.0, 0.0, 0.0, 1.0);
    };
    double hResolution = 1280.0;
    double vResolution = 720.0;
    double fRate = 25.0;
    Gdk::RGBA color;
};

class Project {
    public:
        Project() {};
        Project(ProjectSettings sett) : settings(sett) {}
        
        ProjectSettings& getSettings() { return settings; }
        bool importMedia(std::unique_ptr<ProjectItem> media);
        bool hasMedia();
        ProjectItem* getSelectedMedia();
    private:
        ProjectSettings settings;
        MediaArray projectMedia;
};
