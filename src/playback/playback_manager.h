#pragma once
#include <gtkmm.h>

#ifndef PLAYBACK_MANAGER_H
#define PLAYBACK_MANAGER_H

class PlaybackSource;

class PlaybackManager {
public:
	PlaybackManager();
	PlaybackManager(PlaybackSource *source);
	void play();
	void pause();
	void stop();
	void next();
	void prev();
	void first();
	void last();
	void gotoFrame(int num);
	void gotoTime(int msec);
	void setLoop(bool);
	void setSource(PlaybackSource *src);
private:
	bool update();
	PlaybackSource *m_source;

	sigc::connection m_update_connection;
	bool playing = false;
	bool loop = false;
};

#endif