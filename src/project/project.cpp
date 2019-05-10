#include <iostream>

#include "project.h"

int Project::importMedia(std::unique_ptr<ProjectItem> media) {
    // Add the media to the project
    projectMedia.push_back(std::move(media));
    return projectMedia.size() - 1;
}

void Project::setSelectedMedia(int id) {
    selectedMedia = id;
}

ProjectItem* Project::getSelectedMedia() {
    if (projectMedia.size() > 0) {
        return getMediaById(selectedMedia);
    }
    return nullptr;
}

ProjectItem* Project::getMediaById(int id) {
    return projectMedia.at(id).get();
}

bool Project::hasMedia() {
    return !projectMedia.empty();
}
