//made by K. Kameník - Meziksicht, 2025.
#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
#include <opencv2/opencv.hpp>
#include <string>

class FFmpegReader {
public:
    FFmpegReader(const std::string& url);
    ~FFmpegReader();

    bool receiveFrame(cv::Mat& outputFrame);

private:
    AVFormatContext* fmt_ctx = nullptr;
    AVCodecContext* codec_ctx = nullptr;
    SwsContext* sws_ctx = nullptr;
    int video_stream_index = -1;
    AVFrame* frame = nullptr;
    AVPacket* packet = nullptr;
    int width = 0;
    int height = 0;
};
