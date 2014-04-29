#include "PIDAdaptation.h"

void PIDAdaptation::setDownloadTime(double time)
{
	this->T_previous = vk_previous/time;
	this->time = time;
	this->throughputs.pop_back();
	this->throughputs.push_front(this->T_previous);
}

void PIDAdaptation::setBufferStatus(double status)
{
	this->bufferContents = (int)status;
}

double PIDAdaptation::ControlModule (double Kp, double q_tks, double q_ref, double delta, double q_tks_previous, double Vl, double vk_previous, double W)
{
	/* Buffer Size Adjustment */
	double exp_factor = exp( Kp * (q_tks - q_ref) );
	double Fq = 2.0 * exp_factor;

	/* Buffer Trend Adjustment */
	double Ft = delta / ( delta - (q_tks - q_tks_previous) );

	/* Video Chunk Size Adjustment */
	double Fv = Vl/(vk_previous+W) + W/(Vl+W);
	Fv = 1.0;

	double result = Fq * Ft * Fv;
	return result;
}

int PIDAdaptation::RateSelector(double computed_rate, std::list<int> representations)
{
	int index = 0;
	list<int>::const_iterator iterator;

	for(iterator = representations.begin(); iterator != representations.end(); ++iterator)
	{
		double value = *iterator;
		if(computed_rate >= value)
			index++;
		else
			break;
	}

	index -= 1;

	if(index < 0)
		index = 0;
	if(index >= representations.size())
		index = representations.size()-1;

	return index;
}

double PIDAdaptation::EstimateThroughput(list<double> throughputs)
{
	double max = 0;
	double min = DBL_MAX;
	double sum = 0;

	for(list<double>::const_iterator iterator = throughputs.begin(); iterator != throughputs.end(); ++iterator)
	{
		if(*iterator > max)
			max = *iterator;
		if(*iterator < min)
			min = *iterator;
		sum += *iterator;
	}

	sum -= (min+max);
	double result = sum/(throughputs.size()-2);
	return result;
}

PIDAdaptation::PIDAdaptation(IMPD* mpd)
{
	this->baseurls.push_back(this->mpd->GetBaseUrls().at(0));

    this->representation = this->mpd->GetPeriods().at(0)->GetAdaptationSets().at(0)->GetRepresentation().at(0);

	vector<IRepresentation *>::const_iterator iterator = this->mpd->GetPeriods().at(0)->GetAdaptationSets().at(0)->GetRepresentation().begin();
	for(; iterator != this->mpd->GetPeriods().at(0)->GetAdaptationSets().at(0)->GetRepresentation().end(); ++iterator)
	{
		int bandwidth = (*iterator)->GetBandwidth();
		this->representations.push_back(bandwidth);
	}

	int base_throughput = 300000;

	for(int i = 0; i < 6; i++)
		this->throughputs.push_back(base_throughput);

	Kp = 1;
	bufferContents = 0;
	q_tks = bufferContents*2;
	q_tks_previous = q_tks;
	time = 2;
	q_ref = 10*2;
	delta = 2;
	Vl = 1;
	vk_previous = 0;
	vk_predicted = 0;
	Fk = 1;
	W = 2;
	T_predicted = base_throughput;
	T_previous = base_throughput;
	counter = 0;
	m = 2;
	index = 0;
	start_counter = 0;
}

IBufferItem* PIDAdaptation::getSegment(int sequenceNumber)
{

	T_predicted = this->EstimateThroughput(throughputs);
	Fk = ControlModule(this->Kp, this->q_tks, this->q_ref, this->delta, this->q_tks_previous, this->Vl, this->vk_previous, this->W); 
	vk_predicted = Fk * this->T_predicted;
	vk_previous = this->mpd->GetPeriods().at(0)->GetAdaptationSets().at(0)->GetRepresentation().at(index)->GetBandwidth();
	q_tks_previous = q_tks;
	q_tks = delta*bufferContents + delta - time;

	if(abs(q_tks - q_tks_previous) > 2)
		q_tks_previous = q_tks+0.1;
	if(abs(q_tks - q_tks_previous) < 0.01)
		q_tks_previous = q_tks+0.1;

	if(q_tks < q_ref/2)
	{
		index = this->RateSelector(T_previous, this->representations);
	}
	else if((int)vk_predicted > (int)vk_previous)
	{
		counter++;
		if(counter > m)
		{
			counter = 0;
			index = this->RateSelector(T_predicted, this->representations);
		}
	}
	else if((int)vk_predicted < (int)vk_previous)
	{
		counter = 0;
	}
	else
		index = this->RateSelector(vk_previous, this->representations);

	std::cout<<"index: "<<index<<"\n";

    ISegment *seg = NULL;
	this->representation = this->mpd->GetPeriods().at(0)->GetAdaptationSets().at(0)->GetRepresentation().at(index);
    if(sequenceNumber >= this->representation->GetSegmentList()->GetSegmentURLs().size() + 1)
        return NULL;

    if(sequenceNumber == 0)
        seg = this->representation->GetSegmentBase()->GetInitialization()->ToSegment(this->baseurls);
    else
        seg = this->representation->GetSegmentList()->GetSegmentURLs().at(sequenceNumber - 1)->ToMediaSegment(this->baseurls);

    IBufferItem *media = new BufferItem(seg, this->representation);

    return media;
}