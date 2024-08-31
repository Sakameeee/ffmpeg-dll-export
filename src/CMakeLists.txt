set(FFMPEG_SRCS
    ffmpeg.cpp
)

set(MODULE_NAME "ffmpeg")

if(LINK_STATIC_LIBS)
    set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg-static-build/lib/pkgconfig")

    # 查找pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(AVCODEC REQUIRED IMPORTED_TARGET libavcodec)
    pkg_check_modules(AVFORMAT REQUIRED IMPORTED_TARGET libavformat)
    pkg_check_modules(AVUTIL REQUIRED IMPORTED_TARGET libavutil)

    # 包含FFmpeg头文件目录
    include_directories(${AVCODEC_INCLUDE_DIRS} ${AVFORMAT_INCLUDE_DIRS} ${AVUTIL_INCLUDE_DIRS})

    # 生成DLL
    add_library(${MODULE_NAME} SHARED ${FFMPEG_SRCS})

    # 链接FFmpeg静态库
    target_link_libraries(${MODULE_NAME}
        PkgConfig::AVCODEC
        PkgConfig::AVFORMAT
        PkgConfig::AVUTIL

        -static
        -lbz2
        -lz
        -liconv
    )
endif()

if(LINK_SHARED_LIBS)
    set(FFMPEG_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg-shared-build/include)
    set(FFMPEG_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg-shared-build/lib)
    set(FFMPEG_LIBS avformat avcodec avutil)

    # 添加所有需要的包含目录
    include_directories(
        ${FFMPEG_INCLUDE}
        ${FFMPEG_INCLUDE}/libavformat
        ${FFMPEG_INCLUDE}/libavcodec
        ${FFMPEG_INCLUDE}/libavutil
    )
    # 添加静态库目录
    link_directories(${FFMPEG_LIBS_DIR})

    #指定所添加依赖动态库的导入路径
    add_library( avformat SHARED IMPORTED )
    set_target_properties( avformat PROPERTIES IMPORTED_IMPLIB ${FFMPEG_LIBS_DIR}/libavformat.dll.a )

    add_library( avcodec SHARED IMPORTED )
    set_target_properties( avcodec PROPERTIES IMPORTED_IMPLIB ${FFMPEG_LIBS_DIR}/libavcodec.dll.a )
    
    add_library( avutil SHARED IMPORTED )
    set_target_properties( avutil PROPERTIES IMPORTED_IMPLIB ${FFMPEG_LIBS_DIR}/libavutil.dll.a )

    add_library(${MODULE_NAME} SHARED ${FFMPEG_SRCS})
    target_link_libraries(${MODULE_NAME} ${FFMPEG_LIBS})
endif()