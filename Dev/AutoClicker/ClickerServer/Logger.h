#pragma once

#include "cpprest/filestream.h"

using namespace utility;

class Logger
{
public:
	Logger(string_t fileName);
	~Logger();
	void LogUpgrade(int64_t frame, string_t upgradeName);
	void Dump(string_t endGameDump);
private:
	bool isLogging = false;

	Concurrency::task <Concurrency::streams::ostream > streamTask;
};

