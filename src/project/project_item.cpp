
#include <GL/glew.h>
#include <GL/gl.h>
#include <gtkmm.h>

extern "C" {

#include "libavformat/avformat.h"

}

#include "project_item.h"
#include "ffmpeg_video_clip.h"

#include <iostream>

ProjectItem::ProjectItem(std::string name) {
    this->m_name = name;
}

ProjectItem::~ProjectItem()
{}

std::string ProjectItem::getName() {
    return m_name;
}

VideoItem::VideoItem(std::string name) : ProjectItem(name) {}

VideoItem::~VideoItem()
{}

int VideoItem::get_num_frames() {
	return m_clip->getFrameCount();
}

double VideoItem::get_frame_rate() {
	return m_clip->getFrameRate();
}

double VideoItem::get_current_time_ms() {
	return m_clip->getCurrentTime();
}

int VideoItem::get_current_frame() {
	m_clip->getCurrentFrame();
}

bool VideoItem::set_next_frame(int frame) {
	m_clip->seekTo(frame);
	return true;
}

Glib::RefPtr<Gdk::Pixbuf> VideoItem::get_pixbuf() {
    return this->m_pixelBuffer;
}

bool VideoItem::load_next_frame(ContextManager *context) {
	if (!m_clip->readNextFrame()) {
        return false;
    }

	context->make_current();

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_clip->getWidth(), m_clip->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, m_clip->getFrame()->data[0]);
	glBindTexture(GL_TEXTURE_2D, 0);

	context->render_media(this);
    return true;
}

bool VideoItem::load_media(std::string file_name, ContextManager *context, std::string &error) {

    m_clip = std::make_unique<FFmpegVideoClip>(file_name);

    if (!m_clip->isLoaded()) {
        error = m_clip->getError();
        return false;
    }

    // Make sure we're using the correct drawing context before allocating textures
    context->make_current();

    m_clip->readNextFrame();
    AVFrame *clipFrame = m_clip->getFrame();

    // Load the first frame into the pixbuf
    auto imgSize = m_clip->getBufferSize();
    m_pixelBuffer = Gdk::Pixbuf::create_from_data(clipFrame->data[0], Gdk::Colorspace::COLORSPACE_RGB,
                                                  FALSE, 8, m_clip->getWidth(), m_clip->getHeight(),
                                                  clipFrame->linesize[0]);

	int tileSize = 64;
    m_pixelBuffer = m_pixelBuffer->scale_simple(tileSize, ((double) m_pixelBuffer->get_height()/m_pixelBuffer->get_width())*tileSize, Gdk::InterpType::INTERP_BILINEAR);
    
    // Store the VideoItem texture ID
    glGenTextures( 1, &texture_id);
    glBindTexture( GL_TEXTURE_2D, texture_id );
    
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    
    // Note that OpenCV uses BGR internally, so we use this in our call to glTexImage2D. This also allocates memory for downloading later textures
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_clip->getWidth(), m_clip->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, clipFrame->data[0]);
    
    // Unbind buffer to reset state
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    this->loaded = true;
    return true;
}

const unsigned int VideoItem::getTexId() {
    return this->texture_id;
}

bool VideoItem::isLoaded() {
    return this->loaded;
}
