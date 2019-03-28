
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
	
	timerId++;
	sigc::slot<bool> update_slot = sigc::bind(sigc::mem_fun(*this, &PlaybackManager::update), timerId);
	m_update_connection = Glib::signal_timeout().connect(update_slot, timeBetweenFrames);

	m_signal_source_changed.emit(true);
	m_signal_playback.emit(false);

	source_loaded = true;
}

sigc::signal<void, bool> PlaybackManager::signal_source_changed() {
	return m_signal_source_changed;
}

sigc::signal<void, bool> PlaybackManager::signal_playback() {
	return m_signal_playback;
}

void PlaybackManager::clearSource() {
	m_source = nullptr;
	this->playing = false;

	source_loaded = false;
	m_signal_source_changed.emit(false);
}

bool PlaybackManager::isPlaying() {
	return this->playing;
}

bool PlaybackManager::hasSource() {
	return this->source_loaded;
}

void PlaybackManager::play() {
	this->playing = true;
	m_signal_playback.emit(true);
}

void PlaybackManager::pause() {
	this->playing = false;
	m_signal_playback.emit(false);
}

void PlaybackManager::stop() {
	m_source->gotoFrame(0);
	this->playing = false;
	m_signal_playback.emit(false);
}

void PlaybackManager::next() {
	m_source->advanceFrame();
	this->playing = false;
	m_signal_playback.emit(false);
}

void PlaybackManager::prev() {
	m_source->retreatFrame();
	this->playing = false;
	m_signal_playback.emit(false);
}

void PlaybackManager::first() {
	m_source->gotoFrame(0);
	this->playing = false;
	m_signal_playback.emit(false);
}

void PlaybackManager::last() {
	m_source->gotoFrame(m_source->getLengthFrames()-5);
	this->playing = false;
	m_signal_playback.emit(false);
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

bool PlaybackManager::update(int id) {
	if (id < this->timerId) {
		// Rogue timer from previous playback instance
		return false;
	}
	if (this->playing) {
		if (!m_source->advanceFrame()) {
			gotoFrame(0);
			if (!this->loop) {
				m_signal_playback.emit(false);
				this->playing = false;
			}
		}
	}
	return true;
}