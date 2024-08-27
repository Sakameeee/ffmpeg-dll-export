#pragma once

#include <cstdint>

#ifndef FFMPEG_H
#define FFMPEG_H

#ifdef _WIN32
#define FFMPEG_API __declspec(dllexport)
#else
#define FFMPEG_API
#endif

extern "C" {
// 合并音频和视频文件的函数
FFMPEG_API bool merge_video_audio(const char *videoPath, const char *audioPath,
                                  const char *outputPath);
}

#endif // FFMPEG_WRAPPER_H
