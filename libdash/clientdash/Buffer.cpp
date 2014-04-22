#include "Buffer.h"

Buffer::Buffer(int size)
{
	InitializeConditionVariable(&this->full);
    InitializeConditionVariable(&this->empty);
    InitializeCriticalSection(&this->monitorMutex);
	end = false;
	buffer_size = size;
}

Buffer::~Buffer()
{
	DeleteConditionVariable (&this->full);
    DeleteConditionVariable (&this->empty);
    DeleteCriticalSection   (&this->monitorMutex);

	while(!this->buffer.empty())
    {
        delete(this->buffer.front());
        this->buffer.pop();
    }
}
		
void Buffer::push(IBufferItem* item)
{
	EnterCriticalSection(&this->monitorMutex);

    while(this->buffer.size() >= this->buffer_size && !this->end)
        SleepConditionVariableCS(&this->empty, &this->monitorMutex, INFINITE);

    if(this->buffer.size() >= this->buffer_size)
    {
        delete(item);
        LeaveCriticalSection(&this->monitorMutex);
        return;
    }

    this->buffer.push(item);

    WakeAllConditionVariable(&this->full);
    LeaveCriticalSection(&this->monitorMutex);
}

IBufferItem* Buffer::front()
{
	EnterCriticalSection(&this->monitorMutex);

    while(this->buffer.size() == 0 && !this->end)
        SleepConditionVariableCS(&this->full, &this->monitorMutex, INFINITE);

    if(this->buffer.size() == 0)
    {
        LeaveCriticalSection(&this->monitorMutex);
        return NULL;
    }

    LeaveCriticalSection(&this->monitorMutex);

    return this->buffer.front();
}

void Buffer::pop()
{
	EnterCriticalSection(&this->monitorMutex);

    delete(this->buffer.front());
    this->buffer.pop();

    WakeAllConditionVariable(&this->empty);
    LeaveCriticalSection(&this->monitorMutex);
}

void Buffer::setEnd(bool value)
{
	EnterCriticalSection(&this->monitorMutex);

    this->end = value;

    WakeAllConditionVariable(&this->empty);
    WakeAllConditionVariable(&this->full);
    LeaveCriticalSection(&this->monitorMutex);
}

uint32_t Buffer::size()
{
	EnterCriticalSection(&this->monitorMutex);

    uint32_t ret = this->buffer.size();

    LeaveCriticalSection(&this->monitorMutex);

    return ret;
}