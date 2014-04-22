#pragma once
#include <stdint.h>

struct audioFrameProperties
{
	int streamIndex;
	int linesize;
	int sampleRate;
	int channels;
	int samples;
	int chunkSize;
	bool fireError;
	char* errorMessage;
};

class  IAudioObserver
{
    public:
        virtual ~IAudioObserver(){};
        virtual void onAudioDataAvailable(const uint8_t **data, audioFrameProperties* props) = 0;
};
