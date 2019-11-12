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
	ClickerServer(utility::string_t url);

	pplx::task<void> open() { 
		return listener.open(); 
	}
	
	pplx::task<void> close() {
		return listener.close();
	}

	~ClickerServer();

private:

	void handle_get(http_request message);

	http_listener listener;
	ClickerManager clickerManager;
};

