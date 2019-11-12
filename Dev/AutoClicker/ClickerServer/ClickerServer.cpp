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
	this->clickerManager.PostOrder(Order::Tick);
	this->clickerManager.ProcessNextOrder();
	web::json::value currentState = this->clickerManager.GetDataAsJson();
	string_t body = currentState.serialize();
	message.reply(status_codes::OK, body);
}