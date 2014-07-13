#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <Windows.h>
#include "IAudioObserver.h"
#include "IVideoObserver.h"
#include "IDownloader.h"

using namespace std;

struct StreamConfig
{
    AVStream       *stream;
    AVCodecContext *codecContext;
    int             frameCnt;
};

class LibavDecoder
{
    public:
        LibavDecoder(IDownloader *rec);
        virtual ~LibavDecoder();

        bool decode();
        bool init();
        void stop();
        void attachVideoObserver(IVideoObserver *observer);
        void attachAudioObserver(IAudioObserver *observer);
        void notifyVideo(AVFrame *frame, StreamConfig *config);
        void notifyAudio(AVFrame *frame, StreamConfig *config);
        void setFrameRate(uint8_t rate);

    private:
		IDownloader* receiver;
        std::vector <StreamConfig> streamconfigs;
        std::vector <IVideoObserver*> videoObservers;
        std::vector <IAudioObserver*> audioObservers;
        unsigned char* iobuffer;
        int bufferSize;
        int framerate;
        AVFormatContext* avFormatContextPtr;
        AVFrame* picture;
        AVPacket avpkt;

        AVFormatContext* openInput();
        void initStreams(AVFormatContext *ctx);
        StreamConfig* getNextFrame(AVFormatContext *ctx, AVPacket *pkt);
        int decodeFrame(AVFrame *frame, AVPacket *pkt, StreamConfig *streamCfg);
        void freeConfigs();
        void error(std::string errormsg, int errorcode);
        bool errorHappened;
};
