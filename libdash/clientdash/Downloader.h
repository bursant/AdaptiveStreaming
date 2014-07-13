#pragma once
#include "IDownloader.h"
#include "libdash.h"
#include "IBuffer.h"
#include "IAdaptation.h"
#include "Multithreading.h"
#include "BufferItem.h"
#include "boost/date_time/posix_time/posix_time.hpp" 
 
typedef boost::posix_time::ptime Time;
typedef boost::posix_time::time_duration TimeDuration;

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
