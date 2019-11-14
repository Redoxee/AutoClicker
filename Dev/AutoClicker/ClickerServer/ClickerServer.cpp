#include "stdafx.h"
#include "ClickerServer.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

ClickerServer::ClickerServer(utility::string_t url) : listener(url)
{
	this->clickerManager.StartClickerThread();

	listener.support(methods::GET, std::bind(&ClickerServer::handle_get, this, std::placeholders::_1));
}

ClickerServer::~ClickerServer()
{
}

void ClickerServer::handle_get(http_request message)
{
	auto http_get_vars = uri::split_query(message.request_uri().query());
	for (auto current = http_get_vars.begin(); current != http_get_vars.end(); ++current)
	{
		cout << utility::conversions::to_utf8string(current->first) << " = " << utility::conversions::to_utf8string(current->second) << endl;
	}

	this->clickerManager.PostOrder(Order::Tick);
	this->clickerManager.ProcessNextOrder();
	web::json::value currentState = this->clickerManager.GetDataAsJson();
	string_t body = currentState.serialize();
	message.reply(status_codes::OK, body);
}