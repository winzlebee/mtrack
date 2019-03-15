
#include <iostream>

#include "playback_manager.h"
#include "playback_source.h"

PlaybackManager::PlaybackManager() {};

PlaybackManager::PlaybackManager(PlaybackSource *source) {
	setSource(source);
};

void PlaybackManager::setSource(PlaybackSource *src) {
	m_source = src;
	int timeBetweenFrames = (1000.0 / m_source->getFrameRate());
	m_update_connection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &PlaybackManager::update), timeBetweenFrames);
}

void PlaybackManager::play() {
	this->playing = true;
}

void PlaybackManager::pause() {
	this->playing = false;
}

void PlaybackManager::stop() {
	m_source->gotoFrame(0);
	this->playing = false;
}

void PlaybackManager::next() {
	m_source->advanceFrame();
	this->playing = false;
}

void PlaybackManager::prev() {
	m_source->retreatFrame();
	this->playing = false;
}

void PlaybackManager::first() {
	m_source->gotoFrame(0);
	this->playing = false;
}

void PlaybackManager::last() {
	m_source->gotoFrame(m_source->getLengthFrames());
	this->playing = false;
}

void PlaybackManager::gotoFrame(int num) {
	m_source->gotoFrame(num);
}

void PlaybackManager::gotoTime(int msec) {
	gotoFrame(msec/(1000/m_source->getFrameRate()));
}

void PlaybackManager::setLoop(bool setLoop) {
	this->loop = setLoop;
}

bool PlaybackManager::update() {
	if (this->playing) {
		if (!m_source->advanceFrame()) {
			//m_source->gotoFrame(0);
			if (!this->loop) {
				//this->playing = false;
			}
		}
	}
	return true;
}