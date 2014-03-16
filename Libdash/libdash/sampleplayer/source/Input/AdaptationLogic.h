/*
 * AdaptationLogic.h
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#ifndef ADAPTATIONLOGIC_H_
#define ADAPTATIONLOGIC_H_

#include "IMPD.h"
#include "MediaObject.h"
#include "IRepresentation.h"
#include <list>
#include <cmath>
#include <iostream>
#include <cfloat>
#include <ctime>

namespace sampleplayer
{
    namespace input
    {
        class AdaptationLogic
        {
            public:
                AdaptationLogic             (dash::mpd::IMPD *mpd);
                virtual ~AdaptationLogic    ();
				void SendTime(double time);
                MediaObject* GetSegment(uint32_t number);
				void SetBufferContents(int size);

            private:
                dash::mpd::IMPD                     *mpd;
                dash::mpd::IRepresentation          *rep;
                std::vector<dash::mpd::IBaseUrl *>  baseurls;

				double Kp;
				double q_tks;
				double q_tks_previous;
				double q_ref;
				double delta;
				double Vl;
				double vk_previous;
				double vk_predicted;
				double Fk;
				double W;
				double T_predicted;
				double T_previous;
				double index;
				double time;
				int counter;
				int start_counter;
				int m;
				int bufferContents;
				std::list<int> representations;
				std::list<double> throughputs;

				double ControlModule (double Kp, double q_tks, double q_ref, double delta, double q_tks_previous, double Vl, double vk_previous, double W);
				int RateSelector(double computed_rate, std::list<int> representations);
				double EstimateThroughput(std::list<double> throughputs);

		};
    }
}

#endif /* ADAPTATIONLOGIC_H_ */