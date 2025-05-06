//made by K. Kameník - Meziksicht, 2025.
#include "FFmpegReader.h"
#include <iostream>

FFmpegReader::FFmpegReader(const std::string& url) {
    avformat_network_init();

    if (avformat_open_input(&fmt_ctx, url.c_str(), nullptr, nullptr) < 0) {
        throw std::runtime_error("Failed to open input stream: " + url);
    }

    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        throw std::runtime_error("Failed to get stream info");
    }

    for (unsigned int i = 0; i < fmt_ctx->nb_streams; ++i) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }

    if (video_stream_index == -1) {
        throw std::runtime_error("No video stream found");
    }

    AVCodecParameters* codecpar = fmt_ctx->streams[video_stream_index]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);
    if (!codec) {
        throw std::runtime_error("Unsupported codec");
    }

    codec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codec_ctx, codecpar);
    if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
        throw std::runtime_error("Failed to open codec");
    }

    width = codec_ctx->width;
    height = codec_ctx->height;

    frame = av_frame_alloc();
    packet = av_packet_alloc();
    sws_ctx = sws_getContext(
        width, height, codec_ctx->pix_fmt,
        width, height, AV_PIX_FMT_BGR24,
        SWS_BILINEAR, nullptr, nullptr, nullptr
    );
}

FFmpegReader::~FFmpegReader() {
    if (sws_ctx) sws_freeContext(sws_ctx);
    if (frame) av_frame_free(&frame);
    if (packet) av_packet_free(&packet);
    if (codec_ctx) avcodec_free_context(&codec_ctx);
    if (fmt_ctx) avformat_close_input(&fmt_ctx);
    avformat_network_deinit();
}

bool FFmpegReader::receiveFrame(cv::Mat& outputFrame) {
    while (av_read_frame(fmt_ctx, packet) >= 0) {
        if (packet->stream_index == video_stream_index) {
            if (avcodec_send_packet(codec_ctx, packet) >= 0) {
                while (avcodec_receive_frame(codec_ctx, frame) == 0) {
                    outputFrame.create(height, width, CV_8UC3);
                    uint8_t* dst_data[] = { outputFrame.data };
                    int dst_linesize[] = { static_cast<int>(outputFrame.step) };

                    sws_scale(
                        sws_ctx,
                        frame->data, frame->linesize,
                        0, height,
                        dst_data, dst_linesize
                    );

                    av_packet_unref(packet);
                    return true; // Frame ready
                }
            }
        }
        av_packet_unref(packet);
    }
    return false; // No more frames
}
