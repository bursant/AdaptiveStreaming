#pragma once
#include <stdint.h>

class IDownloader
{
    public:
        virtual int IORead(uint8_t *buf, int buf_size) = 0;
		virtual ~IDownloader(){};
};
