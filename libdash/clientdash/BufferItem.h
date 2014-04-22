#pragma once
#include "IBufferItem.h"
#include "libdash.h"

using namespace dash::network;
using namespace dash::mpd;

class BufferItem : public IBufferItem , public IDownloadObserver
{
	public:
		BufferItem(ISegment *segment, IRepresentation *representation);

		bool start();
		void wait();
		int read(uint8_t *data, size_t length);
		int peek(uint8_t *data, size_t length);

		virtual void OnDownloadStateChanged(DownloadState state);
		virtual void OnDownloadRateChanged(uint64_t bytesDownloaded);

	private:
		ISegment *segment;
		IRepresentation *representation;
		DownloadState state;
};