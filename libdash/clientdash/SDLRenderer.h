#pragma once
#include "IVideoObserver.h"
#include <SDL.h>
#include <SDL_thread.h>

class SDLRenderer : public IVideoObserver
{
    public:
        SDLRenderer            ();
        virtual ~SDLRenderer   ();

        bool init               (int width, int height);
        bool processEvents      ();
        bool isQuitKeyPressed   ();

        virtual void onVideoDataAvailable  (const uint8_t **data, videoFrameProperties* props);

    private:
        SDL_Overlay *bmp;
        SDL_Surface *screen;
        SDL_Rect    rect;
        SDL_Event   sdl_event;
        bool        isInit;
        bool        quitKeyPressed;

        struct SwsContext *imgConvertCtx;
};