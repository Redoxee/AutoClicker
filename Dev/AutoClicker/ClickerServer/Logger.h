#pragma once

#include "cpprest/filestream.h"

using namespace utility;

class Logger
{
public:
	Logger(string_t fileName);
	void Log(string_t message);

private:
	bool isLogging = false;
	concurrency::streams::file_stream<unsigned char>* fstream = nullptr;
};

