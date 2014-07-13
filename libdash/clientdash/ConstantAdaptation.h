#pragma once
#include "IAdaptation.h"
#include "BufferItem.h"
#include <vector>

using namespace dash::mpd;
using namespace std;
using namespace boost::log::trivial;
using namespace boost::log::sinks;

class ConstantAdaptation : public IAdaptation
{
    public:
		ConstantAdaptation(IMPD* mpd, int representationID);
        IBufferItem* getSegment(int sequenceNumber);
		void setDownloadTime(double time);
		void setBufferStatus(double status);

	private:
        IMPD* mpd;
        IRepresentation* representation;
        vector<IBaseUrl *> baseurls;
};