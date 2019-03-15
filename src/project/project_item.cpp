
#include <GL/glew.h>
#include <GL/gl.h>

#include "project_item.h"

ProjectItem::ProjectItem(std::string name) {
    this->m_name = name;
}

std::string ProjectItem::getName() {
    return m_name;
}

int VideoItem::get_num_frames() {
	return video->get(cv::CAP_PROP_FRAME_COUNT);
}

double VideoItem::get_frame_rate() {
	return video->get(cv::CAP_PROP_FPS);
}

double VideoItem::get_current_time_ms() {
	return video->get(cv::CAP_PROP_POS_MSEC);
}

int VideoItem::get_current_frame() {
	return video->get(cv::CAP_PROP_POS_FRAMES) - 1;
}

bool VideoItem::set_next_frame(int frame) {
	if (frame > get_num_frames() || frame < 0) return false;
	video->set(cv::CAP_PROP_POS_FRAMES, frame);
	return true;
}

bool VideoItem::load_next_frame(ContextManager *context) {
	cv::Mat newImageFrame;
	if (!video->read(newImageFrame)) return false;

	if (newImageFrame.empty()) return false;

	cv::flip(newImageFrame, newImageFrame, 0);

	context->make_current();
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newImageFrame.cols, newImageFrame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, newImageFrame.data);
	glBindTexture(GL_TEXTURE_2D, 0);

	context->render_media(this);
    return true;
}

void VideoItem::load_media(std::string file_name, ContextManager *context) {

    // Load a video, uses C-style conventions
    video = std::make_unique<cv::VideoCapture>(file_name.c_str());
    if (!video->isOpened()) {
        return;
    }

    // Get the first image of the sequence and store it in OpenGL Memory
    cv::Mat imageFrame;
    (*video) >> imageFrame;
    if (imageFrame.empty()) return;

    // Make sure we're using the correct drawing context before allocating textures
    context->make_current();
    
    // Flip the image, as OpenGL coordinates start at the bottom
    cv::flip(imageFrame, imageFrame, 0);
    
    // Set the pixel store state so it's compatable with OpenCV's image storing method
    glPixelStorei(GL_UNPACK_ALIGNMENT, (imageFrame.step & 3) ? 1 : 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, imageFrame.step/imageFrame.elemSize());
    
    // Store the VideoItem texture ID
    glGenTextures( 1, &texture_id);
    glBindTexture( GL_TEXTURE_2D, texture_id );
    
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    
    // Note that OpenCV uses BGR internally, so we use this in our call to glTexImage2D. This also allocates memory for downloading later textures
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageFrame.cols, imageFrame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, imageFrame.data);
    
    // Unbind buffer to reset state
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    this->loaded = true;
}

const unsigned int VideoItem::getTexId() {
    return this->texture_id;
}

bool VideoItem::isLoaded() {
    return this->loaded;
}
