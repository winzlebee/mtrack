
#include "ffmpeg_video_clip.h"

// FFMPEG imports
extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

}

#include <iostream>

bool FFmpegVideoClip::isLoaded() const
{
    return m_error.empty();
}

double FFmpegVideoClip::getFrameRate() const
{
    return av_q2d(pFormatCtx->streams[videoStreamId]->avg_frame_rate);
}

int64_t FFmpegVideoClip::getFrameCount() const
{
    std::size_t nbf = pFormatCtx->streams[videoStreamId]->nb_frames;

    if (nbf == 0) {
        nbf = (int64_t) floor(getDuration() * getFrameRate() + 0.5);
    }

    return nbf;
}

double FFmpegVideoClip::getCurrentTime() const
{
    return dtsToSec(currentTime) * 1000.0;
}

std::size_t FFmpegVideoClip::getCurrentFrame() const
{
    return frameNumber;
}

int decode(AVCodecContext *avctx, AVFrame *frame, bool &gotFrame, AVPacket *pkt)
{
    int ret;

    gotFrame = false;

    if (pkt) {
        ret = avcodec_send_packet(avctx, pkt);
        // In particular, we don't expect AVERROR(EAGAIN), because we read all
        // decoded frames with avcodec_receive_frame() until done.
        if (ret < 0)
            return ret == AVERROR_EOF ? 0 : ret;
    }

    ret = avcodec_receive_frame(avctx, frame);
    if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
        return ret;
    if (ret >= 0)
        gotFrame = true;

    return 0;
}

double FFmpegVideoClip::dtsToSec(int64_t dts) const
{

    double seconds =
        av_q2d(pFormatCtx->streams[videoStreamId]->time_base) * (double) (dts - pFormatCtx->streams[videoStreamId]->start_time);

    return seconds;
}

double FFmpegVideoClip::getDuration() const
{
    double seconds = (double) pFormatCtx->duration / (double) AV_TIME_BASE;

    if (seconds < 1e-3)
    {
        seconds = (double)pFormatCtx->streams[videoStreamId]->duration * av_q2d(pFormatCtx->streams[videoStreamId]->time_base);
    }

    return seconds;

}

int64_t FFmpegVideoClip::dtsToFrameNumber(int64_t dts) const
{
    double sec = dtsToSec(dts);
    return (int64_t)(getFrameRate() * sec + 0.5);
}

void FFmpegVideoClip::seekTo(int64_t frame)
{
    frame = std::min(frame, getFrameCount());

    // Controls how far back we seek in the video in order to reconstruct a frame.
    // if we don't add this, compressed formats can exhibit artifacts. Increase for higher B-frame intervals.
    int delta = 16;

    // Grab the first frame if we haven't already
    if( firstFrame < 0 && getFrameCount() > 1 ) {
        readNextFrame();
    }

    int64_t _frame_number = frame;

    // std::cout << "Seeking..." << std::endl;
    while (true)
    {
        // Get a time sixteen frames in the past, or at the start of the stream, as compared to our desired frame
        int64_t _frame_number_temp = std::max(_frame_number-delta, (int64_t)0);
        double sec = (double)_frame_number_temp / getFrameRate();
        int64_t time_stamp = pFormatCtx->streams[videoStreamId]->start_time;
        double  time_base  = av_q2d(pFormatCtx->streams[videoStreamId]->time_base);
        time_stamp += (int64_t)(sec / time_base + 0.5);

        // std::printf("Target Frame: %i\nCurrent frame: %i\n timestamp: %.2f", frame, _frame_number_temp, (double) time_stamp*time_base);
        // std::cout << std::endl;

        // Seek to the requested frame, minus 16 frames. Then we can keep reading frames in to get to the correct frame
        if (getFrameCount() > 1) av_seek_frame(pFormatCtx, videoStreamId, time_stamp, AVSEEK_FLAG_BACKWARD);
        avcodec_flush_buffers(pCodecCtx);

        // If we're not at the start of the video
        if( _frame_number > 0 )
        {
            // Read the next frame, which will increment the current frame counter
            readNextFrame();

            // If we've gone further than just stayed at frame 1
            if( _frame_number > 1 )
            {
                // Get the frame at the current time
                frameNumber = dtsToFrameNumber(currentTime) - firstFrame;

                // If the current frame is invalid or higher than our target frame
                if( frameNumber < 0 || frameNumber > _frame_number-1 )
                {
                    // If we've reached the start of the video, or the delta to search is at its maximum, cancel
                    if( _frame_number_temp == 0 || delta >= INT_MAX/4 )
                        break;

                    // Otherwise, make our delta (time in frames to go back) higher by 150%, and again retrieve the frame at this location
                    delta = delta*3/2;
                    continue;
                }

                // If our current frame number is lower than our target, keep reading frames until we get to our frame.
                while( frameNumber < _frame_number-1 )
                {
                    // When we can't read another frame, break the loop regardless. Otherwhise it will terminate when we reach the target frame.
                    if(!readNextFrame())
                        break;
                }
                frameNumber++;
                break;
            }
            else
            {
                // Stay at frame one if it's the only frame (eg. Images)
                frameNumber = 1;
                break;
            }
        }
        else
        {
            // Stay at frame zero if we only have frame zero.
            frameNumber = 0;
            break;
        }
    }
}

int FFmpegVideoClip::getWidth() const
{
    return pCodecCtx->width;
}

int FFmpegVideoClip::getHeight() const
{
    return pCodecCtx->height;
}

bool FFmpegVideoClip::readNextFrame()
{

    if (!pFormatCtx) {
        return false;
    }

    if( pFormatCtx->streams[videoStreamId]->nb_frames > 0 &&
        frameNumber > pFormatCtx->streams[videoStreamId]->nb_frames ) {
        return false;
    }

    currentTime = AV_NOPTS_VALUE;

    struct SwsContext *sws_ctx = NULL;
    bool frameFinished;
    bool valid = false;
    AVPacket packet;

    // Error handling. Sometimes loading the image does not work first go
    const int max_attempts = 1 << 9;
    int num_errors = 0;

    AVPixelFormat pixFormat;
    switch (pCodecCtx->pix_fmt) {
    case AV_PIX_FMT_YUVJ420P :
        pixFormat = AV_PIX_FMT_YUV420P;
        break;
    case AV_PIX_FMT_YUVJ422P  :
        pixFormat = AV_PIX_FMT_YUV422P;
        break;
    case AV_PIX_FMT_YUVJ444P   :
        pixFormat = AV_PIX_FMT_YUV444P;
        break;
    case AV_PIX_FMT_YUVJ440P :
        pixFormat = AV_PIX_FMT_YUV440P;
        break;
    default:
        pixFormat = pCodecCtx->pix_fmt;
        break;
    }

    // initialize SWS context for software scaling
    sws_ctx = sws_getContext(pCodecCtx->width,
        pCodecCtx->height,
        pixFormat,
        pCodecCtx->width,
        pCodecCtx->height,
        AV_PIX_FMT_RGB24,
        SWS_BICUBIC, NULL, NULL, NULL);

    while (!valid) {
        if (av_read_frame(pFormatCtx, &packet) >=0 ) {

            // Is this a packet from the video stream?
            if(packet.stream_index != videoStreamId) {
                num_errors++;
            } else {
                // Decode video frame
                decode(pCodecCtx, pFrame, frameFinished, &packet);

                // Did we get a video frame?
                if(frameFinished) {
                    // Convert the image from its native format to RGB
                    sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
                    pFrame->linesize, 0, pCodecCtx->height,
                    pFrameRGB->data, pFrameRGB->linesize);

                    currentTime = pFrame->pkt_dts;

                    valid = true;
                } else {
                    num_errors++;
                }
            }


            // Free the packet that was allocated by av_read_frame
            av_free_packet(&packet);

        } else {
            num_errors++;
        }

        if (num_errors > max_attempts) {
            break;
        }
    }

    sws_freeContext(sws_ctx);

    if (valid) {
        frameNumber++;
    }

    if (frameFinished && firstFrame < 0) {
        firstFrame = dtsToFrameNumber(currentTime);
    }

    return valid;
}

FFmpegVideoClip::FFmpegVideoClip(const std::string &filename) {

    // Open video file
    if(avformat_open_input(&pFormatCtx, filename.c_str(), NULL, NULL)!=0) {
        m_error = "Could not open media file.";
        return;
    }

    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx, NULL)<0) {
        m_error = "Could not find stream information in file.";
        return;
    }

    // Dump information about file onto standard m_error
    av_dump_format(pFormatCtx, 0, filename.c_str(), 0);

    // Find the last video and audio stream in the file
    for(int i = 0; i < pFormatCtx->nb_streams; i++) {
        AVStream *stream = pFormatCtx->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamId = i;
        }
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamId = i;
        }
    }

    std::cout << "Video Stream: " << videoStreamId << std::endl;
    std::cout << "Audio Stream: " << audioStreamId << std::endl;

    if(videoStreamId == -1) {
        m_error = "Did not find any video stream";
        return;
    }

    // Get a pointer to the codec context for the video stream
    pCodecCtxOrig = pFormatCtx->streams[videoStreamId]->codec;

    // Find the decoder for the video stream
    pCodec=avcodec_find_decoder(pCodecCtxOrig->codec_id);

    if(!pCodec) {
        m_error = "Unsupported codec!";
        return; // Codec not found
    }
    // Copy context
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
        m_error = "Could not copy codec context.";
        return;
    }
    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        m_error = "Failed to open the codec";
        return;
    }

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    if(pFrameRGB == NULL)  {
        m_error = "Could not convert the video frame to RGB.";
        return;
    }


    // Determine required buffer size and allocate buffer
    // TODO: 16-bit per channel colour. We'll only ever use RGB though, for simplicity.
    // Colour scientists, please help me here as this definitely is not my area of expertise!
    bufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width,
                                          pCodecCtx->height, 1);

    buffer = (uint8_t *) av_malloc_array(bufferSize, sizeof(uint8_t));

    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, buffer,
                         AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);


}

FFmpegVideoClip::~FFmpegVideoClip() {
    // Destroy the ffmpeg video object if it has been loaded

    // Free the RGB image
    av_free(buffer);
    av_free(pFrameRGB);

    // Free the YUV frame
    av_free(pFrame);

    // Close the codecs
    avcodec_close(pCodecCtx);
    avcodec_close(pCodecCtxOrig);

    // Close the video file
    avformat_close_input(&pFormatCtx);

}
