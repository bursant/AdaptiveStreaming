#include "Logger.h"

Logger::Logger(string path)
{
	fileHandle.open(path, ofstream::trunc);
}

Logger::~Logger()
{
	fileHandle.close();
}

void Logger::log(string message)
{
	fileHandle<<message<<endl;
}