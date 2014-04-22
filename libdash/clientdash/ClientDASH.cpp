#include <iostream>
#include "libdash.h"
#include "IDASHManager.h"
#include "Logger.h"
#include "Downloader.h"
#include "Buffer.h"
#include "Adaptation.h"
#include "SDLRenderer.h"
#include "LibavDecoder.h"
#include <string>
#include <vector>
#include <boost\program_options.hpp>
#include <boost\algorithm\string\join.hpp>

using namespace std;
using namespace dash;

int main(int argc, char** argv)
{
	string url;
	int bufferSize;

	ILogger* logger = new Logger("clientdash.log");
	logger->log("Logger initialized.");

	namespace po = boost::program_options;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("url", po::value<string>(&url)->default_value("http://15.0.0.6/bunny_Desktop.mpd"), "specify mpd url")
		("buffer", po::value<int>(&bufferSize)->default_value(30), "specify buffer size")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);
		
	if(vm.count("help"))
	{
		cout<<desc<<endl;
		logger->log("Help printed.");
		return 1;
	}

	cout<<url<<endl;
	cout<<bufferSize<<endl;

	IDASHManager* dashManager = CreateDashManager();
	logger->log("DASH Manager created.");

	IBuffer* buffer = new Buffer(bufferSize);
	logger->log("Buffer created.");

	IMPD* mpd = dashManager->Open(strdup(url.c_str()));
	logger->log("MPD file parsed.");

	IAdaptation* adaptation = new Adaptation(mpd);
	logger->log("Adaptation created.");

	IDownloader* downloader = new Downloader(buffer, mpd, dashManager, adaptation);
	logger->log("Downloader started.");

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