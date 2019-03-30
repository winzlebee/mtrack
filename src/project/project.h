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
    int length = 250;
    Gdk::RGBA color;
};

class Project {
    public:
        Project() {};
        Project(ProjectSettings sett) : settings(sett) {}
        
        ProjectSettings& getSettings() { return settings; }

        // Import media into the project. \returns Index of added item.
        int importMedia(std::unique_ptr<ProjectItem> media);
        bool hasMedia();
        ProjectItem* getSelectedMedia();
        ProjectItem* getMediaById(int id);
    private:
        ProjectSettings settings;
        MediaArray projectMedia;
        
};
