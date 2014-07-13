#pragma once
#include "IMPD.h"
#include "IBufferItem.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>

using namespace dash::mpd;

class IAdaptation
{
    public:
        virtual IBufferItem* getSegment(int sequenceNumber) = 0;
		void virtual setDownloadTime(double time) = 0;
		void virtual setBufferStatus(double status) = 0;
};