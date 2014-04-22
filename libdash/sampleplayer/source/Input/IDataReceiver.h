#pragma once
#include <stdint.h>

class IDataReceiver
{
    public:
        virtual ~IDataReceiver(){};
        virtual int IORead(uint8_t *buf, int buf_size) = 0;
};
