#pragma once
#include "IDownloader.h"
#include "libdash.h"
#include "IBuffer.h"
#include "IAdaptation.h"

using namespace dash;
using namespace dash::mpd;

class Downloader : IDownloader
{
    public:
        int IORead(uint8_t *buf, int buf_size);
		Downloader();
		~Downloader();

	private:              
		IDASHManager* manager;
		IMPD* mpd;
		IBuffer* buffer;
		int count;
		IAdaptation* adaptation;
		static void* bufferingProcedure(void* downloader);
};
