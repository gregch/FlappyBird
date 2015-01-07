#include "Logger.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <SDL.h>

// TODO verify 
#ifdef ___WIN32__  
	#include <Windows.h>
#endif

using namespace std;

const std::string Logger::logFileName = "log.txt";

void Logger::log(const string& message)
{
	ofstream logStream;
	logStream.open(logFileName.c_str(), ios::app);
	if (logStream.fail())
		return; // don't throw an exception if log couldn't be opened

	// current date/time based on current system
	time_t now = time(0);
	// convert now to string form
	char *buffer = ctime(&now);
	string dateString = string(buffer);
	dateString.pop_back();

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
#ifdef __WIN32__
	MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
#else
	cerr << "Error: " << msg << endl;
#endif
}
