#include "reseller_login.h"
#include <fmt/format.h>
using namespace reseller;
//add definition of your processing function here
void login::signin(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string username, std::string password)
{

	LOG_INFO << fmt::format("reseller::login::signin username {:s} password {:s}", username, password);


	auto resp = HttpResponse::newHttpJsonResponse(true);
	callback(resp);
}
void login::signout(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{

	LOG_INFO << fmt::format("signout");
	auto resp = HttpResponse::newHttpJsonResponse(true);
	callback(resp);
}

