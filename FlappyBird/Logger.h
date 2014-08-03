#pragma once

#include <string>

class Logger
{
public:
	static const std::string logFileName;

	/* logging */
	static void logSDLError(const std::string &prefix);
	static void logError(const std::string &message);
	static void log(const std::string &message);
};