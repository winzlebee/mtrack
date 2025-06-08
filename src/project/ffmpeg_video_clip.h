#pragma once

#include <string>
#include <cstdint>

struct AVFormatContext;
struct AVCodec;
struct AVCodecContext;
struct AVFrame;

// Represents a single video clip in ffmpeg
class FFmpegVideoClip
{
public:
	FFmpegVideoClip(const std::string &fileLocation);
	~FFmpegVideoClip();

    bool isLoaded() const;
    void seekTo(int64_t frame);
    bool readNextFrame();

    AVFrame *getFrame() const { return pFrameRGB; }
    int64_t getFrameCount() const;
    double getFrameRate() const;

    // Current time in seconds
    double getCurrentTime() const;

    // Duration of the clip in seconds
    double getDuration() const;

    std::size_t getCurrentFrame() const;

    // The size of the image buffer, in unsigned bytes
    int getBufferSize() const { return bufferSize; }

    int getWidth() const;
    int getHeight() const;

    const std::string &getError() { return m_error; }

private:
    double dtsToSec(int64_t dts) const;
    int64_t dtsToFrameNumber(int64_t dts) const;

	AVFormatContext *pFormatCtx = nullptr;

	AVCodecContext *pCodecCtxOrig = nullptr;
	AVCodecContext *pCodecCtx = nullptr;

	const AVCodec *pCodec = nullptr;

	AVFrame *pFrame = nullptr;
	AVFrame *pFrameRGB = nullptr;

    int videoStreamId = -1;
    int audioStreamId = -1;

    int64_t firstFrame = -1;
    int64_t frameNumber = 0;

    // Current time in the video stream, in the video timebase units
    int64_t currentTime;

    // Persistent frame storage
    uint8_t *buffer; // The buffer storing signed bytes representing image values
    int bufferSize; // Size of the image buffer, in bytes

    std::string m_error;
};
