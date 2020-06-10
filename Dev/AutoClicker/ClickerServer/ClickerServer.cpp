#include "stdafx.h"
#include "ClickerServer.h"
#include "boost/algorithm/string.hpp"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

ClickerServer::ClickerServer(utility::string_t url, const json::value& configuration, const string_t logFileName) : listener(url)
{
	this->clickerManager.Initialize(configuration, logFileName);

	this->clickerManager.StartClickerThread();

	listener.support(methods::GET, std::bind(&ClickerServer::HandleGet, this, std::placeholders::_1));
}

ClickerServer::~ClickerServer()
{
}

void ClickerServer::HandleGet(http_request message)
{
	auto http_get_vars = uri::split_query(message.request_uri().query());
	string_t body = U("");

	for (auto current = http_get_vars.begin(); current != http_get_vars.end(); ++current)
	{
		string id = utility::conversions::to_utf8string(web::uri::decode(current->first));
		string parameter = utility::conversions::to_utf8string(web::uri::decode(current->second));
		this->ParseOrderRequest(id, parameter, body);
	}

	message.reply(status_codes::OK, body);
}

void ClickerServer::ParseOrderRequest(std::string& id, std::string& param, string_t& body)
{
	boost::trim(id);
	boost::trim(param);
	
	cout << "Order : " << id << " Parameter : " << param << endl;

	boost::algorithm::to_lower(id);
	if (id == "click")
	{
		this->clickerManager.PostOrder(Order(OrderIdentifier::Click, 0));
	}
	else if (id == "upgrade")
	{
		int generatorIndex = std::stoi(param);
		this->clickerManager.PostOrder(Order(OrderIdentifier::BuyUpgrade, generatorIndex));
	}
	else if (id == "get_state")
	{
		web::json::value currentState = this->clickerManager.GetDataAsJson();
		body = currentState.serialize();
	}
	else if (id == "set_frame_length")
	{
		int frameLength = std::stoi(param);
		this->clickerManager.PostOrder(Order(OrderIdentifier::Meta_FrameWait, frameLength));
	}
	else if (id == "set_update_pause")
	{
		int value = 0;
		if (param == "true")
		{
			value = 1;
		}
		else if (param == "false")
		{
			value = 0;
		}
		else
		{
			value = std::stoi(param);
		}

		this->clickerManager.PostOrder(Order(OrderIdentifier::Meta_PauseUpdate, value));
	}
}
