#pragma once
#include "IMPD.h"
#include "IBufferItem.h"

using namespace dash::mpd;

class IAdaptation
{
    public:
        virtual IBufferItem* getSegment(int sequenceNumber) = 0;
};