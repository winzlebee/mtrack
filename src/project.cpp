#include <iostream>

#include "project.h"

bool Project::importMedia(std::unique_ptr<MediaDisplay> media) {
    // Add the media to the project
    projectMedia.push_back(std::move(media));
    return true;
}

MediaDisplay* Project::getLoadedMedia() {
    return projectMedia.front().get();
}

bool Project::hasMedia() {
    return !projectMedia.empty();
}
