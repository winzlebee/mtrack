#pragma once

#include <string>

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
    void seekTo(int frame);
    bool readNextFrame();

    AVFrame *getFrame() const { return pFrameRGB; }
    const uint64_t &getFrameCount() const;
    double getFrameRate() const;

    // Current time in milliseconds
    double getCurrentTime() const;

    uint64_t getCurrentFrame() const;

    // The size of the image buffer, in unsigned bytes
    int getBufferSize() const { return bufferSize; }

    int getWidth() const;
    int getHeight() const;

    const std::string &getError() { return m_error; }

private:
	AVFormatContext *pFormatCtx = nullptr;

	AVCodecContext *pCodecCtxOrig = nullptr;
	AVCodecContext *pCodecCtx = nullptr;

	AVCodec *pCodec = nullptr;

	AVFrame *pFrame = nullptr;
	AVFrame *pFrameRGB = nullptr;

    int videoStreamId = -1;
    int audioStreamId = -1;

    int frameNumber = 0;

    // Persistent frame storage
    uint8_t *buffer; // The buffer storing signed bytes representing image values
    int bufferSize; // Size of the image buffer, in bytes

    std::string m_error;
};