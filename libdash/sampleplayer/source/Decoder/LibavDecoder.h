#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <Windows.h>
#include "IAudioObserver"

struct StreamConfig
{
    AVStream       *stream;
    AVCodecContext *codecContext;
    int             frameCnt;
};

class LibavDecoder
{
    public:
        LibavDecoder                     (input::IDataReceiver *rec);
        virtual ~LibavDecoder            ();

        bool decode                 ();
        bool init                   ();
        void stop                   ();
        void attachVideoObserver    (renderer::IVideoObserver *observer);
        void attachAudioObserver    (renderer::IAudioObserver *observer);
        void notifyVideo            (AVFrame *frame, StreamConfig *config);
        void notifyAudio            (AVFrame *frame, StreamConfig *config);
        void setFrameRate           (uint8_t rate);

    private:
        input::IDataReceiver                    *receiver;
        std::vector <StreamConfig>              streamconfigs;
        std::vector <renderer::IVideoObserver*> videoObservers;
        std::vector <renderer::IAudioObserver*> audioObservers;
        unsigned char                           *iobuffer;
        int                                     bufferSize;
        int                                     framerate;
        AVFormatContext                         *avFormatContextPtr;
        AVFrame                                 *picture;
        AVPacket                                avpkt;

        AVFormatContext*    openInput       ();
        void                initStreams     (AVFormatContext *ctx);
        StreamConfig*       getNextFrame    (AVFormatContext *ctx, AVPacket *pkt);
        int                 decodeFrame     (AVFrame *frame, AVPacket *pkt, StreamConfig *streamCfg);
        void                freeConfigs     ();
        void                error           (std::string errormsg, int errorcode);
        bool                errorHappened;
};
