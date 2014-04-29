#include "ConstantAdaptation.h"

ConstantAdaptation::ConstantAdaptation(IMPD* mpd, int representationID)
{
	this->mpd = mpd;
	this->baseurls.push_back(this->mpd->GetBaseUrls().at(0));
	this->representation = this->mpd->GetPeriods().at(0)->GetAdaptationSets().at(0)->GetRepresentation().at(representationID);
}

IBufferItem* ConstantAdaptation::getSegment(int sequenceNumber)
{

	ISegment *seg = NULL;

    if(sequenceNumber >= this->representation->GetSegmentList()->GetSegmentURLs().size() + 1)
        return NULL;

    if(sequenceNumber == 0)
        seg = this->representation->GetSegmentBase()->GetInitialization()->ToSegment(this->baseurls);
    else
        seg = this->representation->GetSegmentList()->GetSegmentURLs().at(sequenceNumber - 1)->ToMediaSegment(this->baseurls);

    IBufferItem* item = new BufferItem(seg, this->representation);

	boost::log::sources::severity_logger< severity_level > lg;
	BOOST_LOG_SEV(lg, trace) << sequenceNumber << ";" << this->representation->GetBandwidth();

    return item;
}

void ConstantAdaptation::setDownloadTime(double time)
{
}

void ConstantAdaptation::setBufferStatus(double status)
{
}