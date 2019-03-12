#include <iostream>

#include "project.h"

bool Project::importMedia(std::unique_ptr<ProjectItem> media) {
    // Add the media to the project
    projectMedia.push_back(std::move(media));
    return true;
}

ProjectItem* Project::getLoadedMedia() {
    return projectMedia.front().get();
}

bool Project::hasMedia() {
    return !projectMedia.empty();
}
