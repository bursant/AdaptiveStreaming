#pragma once
#include "IAdaptation.h"
#include "BufferItem.h"
#include <vector>
#include <boost/random.hpp>

using namespace dash::mpd;
using namespace std;
using namespace boost::log::trivial;
using namespace boost::log::sinks;

class RandomAdaptation : public IAdaptation
{
    public:
		RandomAdaptation(IMPD* mpd);
        IBufferItem* getSegment(int sequenceNumber);

	private:
        IMPD* mpd;
        IRepresentation* representation;
        vector<IBaseUrl *> baseurls;
		boost::random::mt19937 rng;
};