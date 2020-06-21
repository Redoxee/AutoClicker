#pragma once

#include "ClickerManager.h"
#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/json.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class ClickerServer
{
public:
	ClickerServer() {}
	ClickerServer(utility::string_t url, const json::value& configuration, const string_t logFileName);

	pplx::task<void> open() {
		return listener.open(); 
	}
	
	pplx::task<void> close() {
		return listener.close();
	}

	bool IsTerminated()
	{
		return this->isTerminated;
	}

	~ClickerServer();

private:

	void HandleGet(http_request message);
	void ParseOrderRequest(std::string& id, std::string& parameter, string_t& body);
	
	bool isTerminated = false;
	http_listener listener;
	ClickerManager clickerManager;
};
