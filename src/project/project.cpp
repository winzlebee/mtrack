#include <iostream>

#include "project.h"

int Project::importMedia(std::unique_ptr<ProjectItem> media) {
    // Add the media to the project
    projectMedia.push_back(std::move(media));
    return projectMedia.size() - 1;
}

ProjectItem* Project::getSelectedMedia() {
    return projectMedia.front().get();
}

ProjectItem* Project::getMediaById(int id) {
    return projectMedia.at(id).get();
}

bool Project::hasMedia() {
    return !projectMedia.empty();
}
