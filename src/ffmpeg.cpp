#include "../include/ffmpeg.h"
#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/timestamp.h>

bool merge_video_audio(const char *videoPath, const char *audioPath,
                       const char *outputPath) {
  // 初始化 FFmpeg
  avformat_network_init();

  // 打开视频和音频文件
  AVFormatContext *videoContext = nullptr;
  AVFormatContext *audioContext = nullptr;
  if (avformat_open_input(&videoContext, videoPath, nullptr, nullptr) < 0 ||
      avformat_open_input(&audioContext, audioPath, nullptr, nullptr) < 0) {
    std::cerr << "unable to open input file" << std::endl;
    return false;
  }

  // 创建输出上下文
  AVFormatContext *outputContext = nullptr;
  avformat_alloc_output_context2(&outputContext, nullptr, nullptr, outputPath);
  if (!outputContext) {
    std::cerr << "unable to create output context" << std::endl;
    return false;
  }

  // 复制视频流和音频流到输出上下文
  for (unsigned int i = 0; i < videoContext->nb_streams; ++i) {
    AVStream *inStream = videoContext->streams[i];
    AVStream *outStream = avformat_new_stream(outputContext, nullptr);
    if (!outStream) {
      std::cerr << "unable to allocate stream" << std::endl;
      return false;
    }
    if (avcodec_parameters_copy(outStream->codecpar, inStream->codecpar) < 0) {
      std::cerr
          << "unable to copy parameters from input stream to output stream"
          << std::endl;
      return false;
    }
  }

  for (unsigned int i = 0; i < audioContext->nb_streams; ++i) {
    AVStream *inStream = audioContext->streams[i];
    AVStream *outStream = avformat_new_stream(outputContext, nullptr);
    if (!outStream) {
      std::cerr << "unable to allocate stream" << std::endl;
      return false;
    }
    if (avcodec_parameters_copy(outStream->codecpar, inStream->codecpar) < 0) {
      std::cerr
          << "unable to copy parameters from input stream to output stream"
          << std::endl;
      return false;
    }

    // 手动设置音频流的编码器参数
    if (inStream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
      const AVCodec *codec = avcodec_find_decoder(
          inStream->codecpar->codec_id); // 修正为 const AVCodec *
      AVCodecContext *codecContext = avcodec_alloc_context3(codec);
      avcodec_parameters_to_context(codecContext, inStream->codecpar);
      if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        std::cerr << "unable to open encoder" << std::endl;
        return false;
      }

      // 设置帧大小
      if (codecContext->frame_size > 0) {
        outStream->codecpar->frame_size = codecContext->frame_size;
      }
      avcodec_free_context(&codecContext);
    }
  }

  // 打开输出文件
  if (!(outputContext->oformat->flags & AVFMT_NOFILE)) {
    if (avio_open(&outputContext->pb, outputPath, AVIO_FLAG_WRITE) < 0) {
      std::cerr << "unable to open output file" << std::endl;
      return false;
    }
  }

  // 写入文件头部
  if (avformat_write_header(outputContext, nullptr) < 0) {
    std::cerr << "failed to write header" << std::endl;
    return false;
  }

  // 从输入流写入数据包到输出流
  AVPacket packet;
  while (true) {
    AVFormatContext *inputContext = videoContext;
    if (av_read_frame(inputContext, &packet) < 0) {
      break;
    }
    AVStream *inStream = inputContext->streams[packet.stream_index];
    AVStream *outStream = outputContext->streams[packet.stream_index];
    av_packet_rescale_ts(&packet, inStream->time_base, outStream->time_base);
    packet.pos = -1;
    if (av_interleaved_write_frame(outputContext, &packet) < 0) {
      std::cerr << "failed to write frame" << std::endl;
      return false;
    }
    av_packet_unref(&packet);
  }

  // 写入文件尾部
  av_write_trailer(outputContext);

  // 清理资源
  avformat_close_input(&videoContext);
  avformat_close_input(&audioContext);
  if (!(outputContext->oformat->flags & AVFMT_NOFILE)) {
    avio_closep(&outputContext->pb);
  }
  avformat_free_context(outputContext);

  return true;
}
}
