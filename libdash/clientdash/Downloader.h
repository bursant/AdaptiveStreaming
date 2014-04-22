#pragma once
#include "IDownloader.h"
#include "libdash.h"
#include "IBuffer.h"
#include "IAdaptation.h"
#include "Multithreading.h"
#include "BufferItem.h"

using namespace dash;
using namespace dash::mpd;

class Downloader : public IDownloader
{
    public:
        int IORead(uint8_t *buf, int buf_size);
		Downloader(IBuffer* buffer, IMPD* mpd, IDASHManager* manager, IAdaptation* adaptation);
		~Downloader();

	private:              
		IDASHManager* manager;
		IMPD* mpd;
		IBuffer* buffer;
		int count;
		IAdaptation* adaptation;
		THREAD_HANDLE bufferingThread;
		static void* bufferingProcedure(void* downloader);
};
