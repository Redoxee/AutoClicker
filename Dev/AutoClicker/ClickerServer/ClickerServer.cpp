#include "stdafx.h"
#include "ClickerServer.h"
#include "boost/algorithm/string.hpp"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

ClickerServer::ClickerServer(utility::string_t url) : listener(url)
{
	this->clickerManager.StartClickerThread();

	listener.support(methods::GET, std::bind(&ClickerServer::HandleGet, this, std::placeholders::_1));
}

ClickerServer::~ClickerServer()
{
}

void ClickerServer::HandleGet(http_request message)
{
	auto http_get_vars = uri::split_query(message.request_uri().query());
	for (auto current = http_get_vars.begin(); current != http_get_vars.end(); ++current)
	{
		string id = utility::conversions::to_utf8string(current->first);
		string parameter = utility::conversions::to_utf8string(current->second);
		this->ParseOrderRequest(id, parameter);
	}

	web::json::value currentState = this->clickerManager.GetDataAsJson();
	string_t body = currentState.serialize();
	message.reply(status_codes::OK, body);
}

void ClickerServer::ParseOrderRequest(string& id, string& param)
{
	boost::algorithm::to_lower(id);
	if (id == "click")
	{
		this->clickerManager.PostOrder(Order(OrderIdentifier::Click, 0));
	}
	else if (id == "generator")
	{
		int generatorIndex = std::stoi(param);
		this->clickerManager.PostOrder(Order(OrderIdentifier::BuyGenerator, generatorIndex));
	}
}