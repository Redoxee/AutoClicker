#include "stdafx.h"
#include "ClickerServer.h"
#include "AutoClicker.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

std::unique_ptr<ClickerServer> g_http;

void on_initialize(const string_t& address)
{
	uri_builder uri(address);
	uri.append_path(U("AutoClicker/"));

	auto addr = uri.to_uri().to_string();
	g_http = std::unique_ptr<ClickerServer>(new ClickerServer(addr));
	g_http->open().wait();

	ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

	return;
}

void on_shutdown()
{
	g_http->close().wait();
	return;
}

int main(int argc, wchar_t *argv[])
{
	utility::string_t port = U("1234");


	utility::string_t address = U("http://localhost:");
	address.append(port);

	on_initialize(address);
	std::cout << "Press ENTER to exit." << std::endl;

	std::string line;
	std::getline(std::cin, line);

	on_shutdown();
	return 0;
}