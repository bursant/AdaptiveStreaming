#include <iostream>
#include "libdash.h"
#include "IDASHManager.h"
#include "ILogger.h"
#include "Logger.h"

using namespace std;
using namespace dash;

int main(int argc, char** argv)
{
	ILogger* logger = new Logger("clientdash.log");

	IDASHManager* dashManager = CreateDashManager();

	return 0;
}