#pragma once
#include "IAdaptation.h"
#include "BufferItem.h"
#include <vector>

using namespace dash::mpd;
using namespace std;
using namespace boost::log::trivial;
using namespace boost::log::sinks;

class LowestAdaptation : public IAdaptation
{
    public:
		LowestAdaptation(IMPD* mpd);
        IBufferItem* getSegment(int sequenceNumber);

	private:
        IMPD* mpd;
        IRepresentation* representation;
        vector<IBaseUrl *> baseurls;
};