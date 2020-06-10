#include "stdafx.h"

#include <map>
#include <fstream>
#include <cpprest/json.h>

#include "ClickerServer.h"
#include "AutoClicker.h"
#include "Logger.h"

using namespace std;

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

std::unique_ptr<ClickerServer> g_http;

bool read_config_file(const string_t& config_file_path, json::value& result)
{
	try
	{
		ifstream file(config_file_path, ios::in);
		if (file.is_open())
		{
			std::stringstream sstream;
			sstream << file.rdbuf();

			string_t stringContent = utility::conversions::to_string_t(sstream.str());
			wcout << "Configuration loaded." << endl;

			result = json::value::parse(stringContent);

			if(result.is_null() || result.size() == 0)
			{
				wcout << "Error while parsing the config file to json." << endl;
				return false;
			}

			if (result.is_null())
			{
				wcout << "json is null" << endl;
				return false;
			}

			return true;
		}
		else
		{
			wcout << "failed to read config file : \"" << config_file_path << "\"" << endl;
		}
	}
	catch (exception& e)
	{
		wcerr << e.what() << endl;
	}

	return false;
}

void start(const string_t& config_filePath, const string_t logFileName)
{
	json::value config;
	if (config_filePath.size() > 0)
	{
		if (!read_config_file(config_filePath, config))
		{
			return;
		}
	}
	else
	{
		wcout << "No config file mode not implemented" << endl;
		return;
	}

	utility::string_t address = U("http://localhost:");
	utility::string_t port = U("1234");

	if (config[U("Address")].is_string())
	{
		address = config[U("Address")].as_string();
	}

	if (config[U("Port")].is_string())
	{
		port = config[U("Port")].as_string();
	}

	address.append(port);

	uri_builder uri(address);
	uri.append_path(U("AutoClicker/"));

	string_t addr = uri.to_uri().to_string();
	g_http = std::unique_ptr<ClickerServer>(new ClickerServer(addr, config, logFileName));

	wcout << "Initialization done." << endl;

	g_http->open().wait();

	ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

	return;
}

void stop()
{
	g_http->close().wait();

	return;
}

int main(int argc, char *argv[])
{
	map<string_t, string_t> arguments;
	string_t configPath = L"";
	string_t logFileName = L"";
	if (argc > 0)
	{
		for (int index = 1; index < (argc - 1); index += 2)
		{
			string_t param = utility::conversions::to_string_t(argv[index]);
			string_t value = utility::conversions::to_string_t(argv[index + 1]);
			arguments[param] = value;
		}

		if (arguments.find(L"Config") != arguments.end())
		{
			configPath = arguments[L"Config"];
		}

		if (arguments.find(L"LogFile") != arguments.end())
		{
			logFileName = arguments[L"LogFile"];
		}

		if (argc % 2 == 0)
		{
			configPath = utility::conversions::to_string_t(argv[argc - 1]);
		}
	}


	start(configPath, logFileName);

	std::wcout << "Press ENTER to exit." << std::endl;
	std::string line;
	std::getline(std::cin, line);

	stop();
	return 0;
}