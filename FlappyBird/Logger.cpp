#include "Logger.h"
#include <fstream>
#include <ctime>
#include <SDL.h>
#include <Windows.h>

using namespace std;

const std::string Logger::logFileName = "log.txt";

void Logger::log(const string& message)
{
	ofstream logStream;
	logStream.open(logFileName, ios_base::app);
	if (logStream.fail())
		return; // don't throw an exception if log couldn't be opened

	// current date/time based on current system
	time_t now = time(0);
	// convert now to string form
	char *buffer = new char[256];
	ctime_s(buffer, 256, &now);
	string dateString = string(buffer);
	dateString.pop_back();
	delete buffer;

	logStream << dateString << ": " << message << endl;
	logStream.close();
}


void Logger::logSDLError(const string &prefix)
{
	string msg = prefix + ": " + SDL_GetError();
	log(msg);
}

void Logger::logError(const string &msg)
{
	MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
}
