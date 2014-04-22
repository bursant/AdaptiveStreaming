#include "BufferItem.h"

using namespace dash::mpd;
using namespace dash::network;

BufferItem::BufferItem(ISegment *segment, IRepresentation *representation)
{
	InitializeConditionVariable(&this->stateChanged);
    InitializeCriticalSection(&this->stateLock);
	this->segment = segment;
	this->representation = representation;
}

BufferItem::~BufferItem()
{
	DeleteConditionVariable (&this->stateChanged);
    DeleteCriticalSection   (&this->stateLock);
}

bool BufferItem::start()
{
	this->segment->AttachDownloadObserver(this);
    return this->segment->StartDownload();
}

void BufferItem::wait()
{
	EnterCriticalSection(&this->stateLock);

    while(this->state != COMPLETED && this->state != ABORTED)
        SleepConditionVariableCS(&this->stateChanged, &this->stateLock, INFINITE);

    LeaveCriticalSection(&this->stateLock);
}

int BufferItem::read(uint8_t *data, size_t length)
{
	return this->segment->Read(data, length);
}

int BufferItem::peek(uint8_t *data, size_t length)
{
	return this->segment->Peek(data, length);
}

void BufferItem::OnDownloadStateChanged(DownloadState state)
{
	EnterCriticalSection(&this->stateLock);

    this->state = state;

    WakeAllConditionVariable(&this->stateChanged);
    LeaveCriticalSection(&this->stateLock);
}

void BufferItem::OnDownloadRateChanged(uint64_t bytesDownloaded)
{
}
