#pragma once

#ifndef PLAYBACK_SOURCE_H
#define PLAYBACK_SOURCE_H

class ProjectItem;
class ContextManager;

class PlaybackSource {
public:
	PlaybackSource(ContextManager *context);
	virtual int getLengthFrames() = 0;
	virtual int getCurrentFrame() = 0;
	virtual double getFrameRate() = 0;

	virtual bool advanceFrame() = 0;
	virtual bool retreatFrame() = 0;
	virtual bool gotoFrame(int frame) = 0;
protected:
	ContextManager *m_context;
};

class MediaPlaybackSource : public PlaybackSource {
public:
	MediaPlaybackSource(ProjectItem *media, ContextManager *context);
	int getLengthFrames();
	int getCurrentFrame();
	double getFrameRate();

	bool advanceFrame();
	bool retreatFrame();
	bool gotoFrame(int frame);

	ProjectItem *getProjectItem();
private:
	ProjectItem *m_media;
};

#endif