#pragma once
#include <stdint.h>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

enum pixelFormat 
{
	yuv420p = 0,
	yuv422p = 4
};

struct videoFrameProperties
{
	int* linesize;
	int frame;
	int streamIndex;
	int framesPerChunk;
	int width;
	int height;
	bool fireError;
	char* errorMessage;
	pixelFormat pxlFmt;
};

class  IVideoObserver
{
    public:
        virtual ~IVideoObserver(){};
        virtual void onVideoDataAvailable (const uint8_t **data, videoFrameProperties* props) = 0;
};
