#pragma once
#include "ILogger.h"
#include <iostream>
#include <fstream>

using namespace std;

class Logger : public ILogger
{
	public:
		Logger(string path);
		~Logger();
		void log(string message);
	
	private:
		ofstream fileHandle;
};
