
#include <iostream>
#include <cmath>

#include "playback_manager.h"
#include "playback_source.h"

PlaybackManager::PlaybackManager() {};

PlaybackManager::PlaybackManager(PlaybackSource *source) {
	setSource(source);
};

void PlaybackManager::setSource(PlaybackSource *src) {
	m_source = src;
	int timeBetweenFrames = round(1000.0 / m_source->getFrameRate());

	std::cout << "New PlaybackSource Loaded." << std::endl;
	std::cout << "Frame Rate: " << m_source->getFrameRate() << std::endl;
	std::cout << "Time per frame: " << timeBetweenFrames << std::endl;
	std::cout << "Realised frame rate: " << 1000.0/timeBetweenFrames << std::endl;

	m_update_connection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &PlaybackManager::update), timeBetweenFrames);

	m_signal_source_changed.emit(true);
}

sigc::signal<void, bool> PlaybackManager::signal_source_changed() {
	return m_signal_source_changed;
}

void PlaybackManager::clearSource() {
	m_source = nullptr;
	this->playing = false;
	m_signal_source_changed.emit(false);
}

bool PlaybackManager::isPlaying() {
	return this->playing;
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