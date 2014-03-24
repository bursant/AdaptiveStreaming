#pragma once

#include "..\libdash\include\libdash.h"
#include "MediaObjectBuffer.h"
#include "Adaptation.hpp"

class Downloader
{
	public:
		Downloader(int buffer_size);
		~Downloader();

		bool Init   (std::string mpdurl);
		int IORead (uint8_t *buf, int buffer_size);

	private:
		dash::IDASHManager  *manager;
		dash::mpd::IMPD     *mpd;
		MediaObjectBuffer   *buffer;
		int                 count;
		uint32_t            maxcapacity;
		AdaptationLogic     *logic;
		THREAD_HANDLE       bufferingThread;

			
		static void* DoBuffering (void *receiver);
		*/
};