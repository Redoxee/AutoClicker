#include "Logger.h"

using namespace utility;
using namespace Concurrency::streams;

Logger::Logger(string_t fileName)
{
	if (fileName.empty())
	{
		this->isLogging = false;
		return;
	}

	this->isLogging = true;
	this->streamTask = file_stream<unsigned char>::open_ostream(fileName);
}

void Logger::LogUpgrade(int64_t frame, string_t upgradeName)
{
	if (this->isLogging)
	{
		Concurrency::streams::ostream stream = this->streamTask.get();
		stream.print("Frame : ");
		stream.print(frame);
		stream.print(" | Upgrade : ");
		stream.print_line(upgradeName);
	}
}

void Logger::Dump(string_t dumpData)
{
	if (this->isLogging)
	{
		Concurrency::streams::ostream stream = this->streamTask.get();
		stream.print_line(dumpData);
	}
}

Logger::~Logger()
{
	if (this->isLogging)
	{
		this->streamTask.get().close();
	}
}