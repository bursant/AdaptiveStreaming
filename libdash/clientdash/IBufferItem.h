#pragma once
#include <inttypes.h>

class IBufferItem
{
	public:
		virtual bool start() = 0;
        virtual void wait() = 0;
        virtual int read(uint8_t* data, size_t length) = 0;
        virtual int peek(uint8_t* data, size_t length) = 0;
};