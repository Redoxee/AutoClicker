#include "stdafx.h"
#include "ClickerServer.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

ClickerServer::ClickerServer(utility::string_t url) : m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&ClickerServer::handle_get, this, std::placeholders::_1));
}

ClickerServer::~ClickerServer()
{
}

void ClickerServer::handle_get(http_request message)
{
	cout << "Hello World" << endl;
	string_t bod = U("HI");
	message.reply(status_codes::OK, bod);
}