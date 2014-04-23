#include <iostream>
#include "libdash.h"
#include "IDASHManager.h"
#include "Downloader.h"
#include "Buffer.h"
#include "Adaptation.h"
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

namespace logs = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace po = boost::program_options;
namespace sinks = boost::log::sinks;

using namespace std;
using namespace dash;
using namespace boost::log::trivial;

int main(int argc, char** argv)
{
	string url;
	int bufferSize;

	logs::add_file_log
    (
        keywords::file_name = "clientdash.log",                                       
        keywords::rotation_size = 100 * 1024 * 1024,                                  
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), 
        keywords::format = "[%TimeStamp%]: %Message%"                                 
    );

	logs::add_common_attributes();
	src::severity_logger< severity_level > lg;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("url", po::value<string>(&url), "specify mpd url")
		("buffer", po::value<int>(&bufferSize)->default_value(30), "specify buffer size")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);
		
	if(vm.count("help"))
	{
		cout<<desc<<endl;
		BOOST_LOG_SEV(lg, info) << "Help printed.";
		return 1;
	}

	IDASHManager* dashManager = CreateDashManager();
	BOOST_LOG_SEV(lg, info) << "DASH Manager created.";

	IBuffer* buffer = new Buffer(bufferSize);
	BOOST_LOG_SEV(lg, info) << "Thread-safe buffer created.";

	IMPD* mpd = dashManager->Open(strdup(url.c_str()));
	BOOST_LOG_SEV(lg, info) << "MPD file parsed.";

	IAdaptation* adaptation = new Adaptation(mpd);
	BOOST_LOG_SEV(lg, info) << "Adaptation created.";

	IDownloader* downloader = new Downloader(buffer, mpd, dashManager, adaptation);
	BOOST_LOG_SEV(lg, info) << "Downloader started.";

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