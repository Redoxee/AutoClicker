#include "stdafx.h"
#include "ClickerServer.h"
#include "AutoClicker.h"

#include <map>
#include <fstream>

using namespace std;

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


std::unique_ptr<ClickerServer> g_http;

void on_initialize(const string_t& address, const string_t& config_filePath)
{
	uri_builder uri(address);
	uri.append_path(U("AutoClicker/"));

	string_t addr = uri.to_uri().to_string();
	g_http = std::unique_ptr<ClickerServer>(new ClickerServer(addr, config_filePath));
	g_http->open().wait();

	ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

	return;
}

void on_shutdown()
{
	g_http->close().wait();
	return;
}

int main(int argc, char *argv[])
{
	map<string_t, string_t> arguments;
	string_t configPath = U("");
	
	if (argc > 0)
	{
		for (int index = 1; index < (argc - 1); index += 2)
		{
			string_t param = utility::conversions::to_string_t(argv[index]);
			string_t value = utility::conversions::to_string_t(argv[index + 1]);
			arguments[param] = value;
		}

		if (argc % 2 == 0)
		{
			configPath = utility::conversions::to_string_t(argv[argc - 1]);
		}
	}

	if (configPath.size() > 0)
	{

	}
	
	utility::string_t port = U("1234");

	utility::string_t address = U("http://localhost:");
	address.append(port);

	on_initialize(address, configPath);
	std::wcout << "Press ENTER to exit." << std::endl;

	std::string line;
	std::getline(std::cin, line);

	on_shutdown();
	return 0;
}