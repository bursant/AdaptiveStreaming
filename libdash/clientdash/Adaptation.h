#pragma once
#include "IAdaptation.h"
#include <vector>

using namespace dash::mpd;
using namespace std;

class Adaptation : public IAdaptation
{
    public:
		Adaptation(IMPD* mpd);
        IBufferItem* getSegment(int sequenceNumber);

	private:
        IMPD* mpd;
        IRepresentation* representation;
        vector<IBaseUrl *> baseurls;
};