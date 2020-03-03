#include "../project/project_item.h"

#include "playback_source.h"

PlaybackSource::PlaybackSource(ContextManager *context) : m_context(context) {};

MediaPlaybackSource::MediaPlaybackSource(ProjectItem *media, ContextManager *context) : PlaybackSource(context), m_media(media) {};

int MediaPlaybackSource::getLengthFrames() {
	return m_media->get_num_frames();
}

int MediaPlaybackSource::getCurrentFrame() {
	return m_media->get_current_frame();
}

double MediaPlaybackSource::getFrameRate() {
	return m_media->get_frame_rate();
}

bool MediaPlaybackSource::advanceFrame() {
	return m_media->load_next_frame(m_context);
}

bool MediaPlaybackSource::retreatFrame() {
	bool next = m_media->set_next_frame(getCurrentFrame() - 2);
	return next && m_media->load_next_frame(m_context);
}

bool MediaPlaybackSource::gotoFrame(int frame) {
	return m_media->set_next_frame(frame) && m_media->load_next_frame(m_context);
}