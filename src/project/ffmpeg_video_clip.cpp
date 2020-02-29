
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
    return !m_error.empty();
}

double FFmpegVideoClip::getFrameRate() const
{
    return av_q2d(pFormatCtx->streams[videoStreamId]->avg_frame_rate);
}

const uint64_t &FFmpegVideoClip::getFrameCount() const
{
    return pFormatCtx->streams[videoStreamId]->nb_frames;
}

double FFmpegVideoClip::getCurrentTime() const
{
    double totalTime = 
        av_q2d(pFormatCtx->streams[videoStreamId]->time_base) *
        double(pFormatCtx->streams[videoStreamId]->duration) * 1000.0;

    return (static_cast<double>(getCurrentFrame()) / static_cast<double>(getFrameCount())) * totalTime;
}

uint64_t FFmpegVideoClip::getCurrentFrame() const
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

void FFmpegVideoClip::seekTo(int frame)
{
    av_seek_frame(pFormatCtx, videoStreamId, frame, AVSEEK_FLAG_FRAME);
    frameNumber = frame;
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
    struct SwsContext *sws_ctx = NULL;
    bool frameFinished;
    AVPacket packet;
    // initialize SWS context for software scaling
    sws_ctx = sws_getContext(pCodecCtx->width,
        pCodecCtx->height,
        pCodecCtx->pix_fmt,
        pCodecCtx->width,
        pCodecCtx->height,
        AV_PIX_FMT_RGB24,
        SWS_BICUBIC, NULL, NULL, NULL);

    if (av_read_frame(pFormatCtx, &packet) >=0 ) {

        // Is this a packet from the video stream?
        if(packet.stream_index == videoStreamId) {
            // Decode video frame
            decode(pCodecCtx, pFrame, frameFinished, &packet);
            
            // Did we get a video frame?
            if(frameFinished) {
                // Convert the image from its native format to RGB
                sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
                pFrame->linesize, 0, pCodecCtx->height,
                pFrameRGB->data, pFrameRGB->linesize);

                frameNumber++;
            }
        }
            
        // Free the packet that was allocated by av_read_frame
        av_free_packet(&packet); 

        return true;
    }


    seekTo(0);
    return false;
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
    pCodecCtx = pFormatCtx->streams[videoStreamId]->codec;

    // Find the decoder for the video stream
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
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