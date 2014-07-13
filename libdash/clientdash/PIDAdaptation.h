#include "IAdaptation.h"
#include "BufferItem.h"
#include <vector>

using namespace dash::mpd;
using namespace std;
using namespace boost::log::trivial;
using namespace boost::log::sinks;

class PIDAdaptation : public IAdaptation
{
    public:
		PIDAdaptation(IMPD* mpd);
        IBufferItem* getSegment(int sequenceNumber);
		void setDownloadTime(double time);
		void setBufferStatus(double status);

	private:
        IMPD* mpd;
        IRepresentation* representation;
        vector<IBaseUrl *> baseurls;
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