#include <iostream>
#include "libdash.h"
#include "IDASHManager.h"
#include "Downloader.h"
#include "Buffer.h"
#include "RandomAdaptation.h"
#include "PIDAdaptation.h"
#include "SmoothAdaptation.h"
#include "ConstantAdaptation.h"
#include "SDLRenderer.h"
#include "LibavDecoder.h"
#include <string>
#include <vector>
#include <boost\program_options.hpp>
#include <boost\algorithm\string\join.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/attributes.hpp>

namespace logs = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace po = boost::program_options;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;

using namespace std;
using namespace dash;
using namespace boost::log::trivial;

void initLogging(string logpath)
{
	logs::add_common_attributes();
	logs::add_file_log
    (
        keywords::file_name = logpath,
        keywords::format = "%TimeStamp%;%Message%",
		keywords::auto_flush = true
    );
}

int main(int argc, char** argv)
{
	string url;
	int bufferSize;
	string logpath = "";
	string adaptationFlag = "";
	int repID;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("url,u", po::value<string>(&url), "specify mpd url")
		("buffer,b", po::value<int>(&bufferSize)->default_value(30), "specify buffer size")
		("log,l", po::value<string>(&logpath)->default_value("clientdash.log"), "specify log path")
		("adaptation,a", po::value<string>(&adaptationFlag)->default_value("Constant"), "specify adaptation algorithm")
		("repID,r", po::value<int>(&repID)->default_value(0), "specify representation for constant adaptation")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);
	
	initLogging(logpath);

	src::severity_logger< severity_level > lg;

	if(vm.count("help"))
	{
		cout<<desc<<endl;
		return 1;
	}

	IDASHManager* dashManager = CreateDashManager();

	IBuffer* buffer = new Buffer(bufferSize);

	IMPD* mpd = dashManager->Open(strdup(url.c_str()));

	IAdaptation* adaptation;
	if(adaptationFlag == "Random")
		adaptation= new RandomAdaptation(mpd);
	else if(adaptationFlag == "Constant")
		adaptation= new ConstantAdaptation(mpd, repID);
	else if(adaptationFlag == "PID")
		adaptation = new PIDAdaptation(mpd);
	else if(adaptationFlag == "Smooth")
		adaptation = new SmoothAdaptation(mpd);

	IDownloader* downloader = new Downloader(buffer, mpd, dashManager, adaptation);

	SDLRenderer *renderer = new SDLRenderer();
	LibavDecoder *decoder = new LibavDecoder(downloader);

	decoder->attachVideoObserver(renderer);
    decoder->setFrameRate(24);
    decoder->init();
    
    bool eos = false;

    while(!renderer->isQuitKeyPressed() && !eos)
    {
        eos = !decoder->decode();
        renderer->processEvents();
    }

    decoder->stop();

	return 0;
}