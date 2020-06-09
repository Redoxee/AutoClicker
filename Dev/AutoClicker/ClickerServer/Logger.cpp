#include "Logger.h"

using namespace utility;
using namespace Concurrency::streams;

Logger::Logger(string_t fileName)
{
	Concurrency::task<ostream> testTask = file_stream<unsigned char>::open_ostream(L"test.txt");
	ostream stream = testTask.get();
	stream.print_line("Hello world");
	stream.close();
}