#include "admin_login.h"
#include <fmt/format.h>
using namespace admin;
//add definition of your processing function here
void login::signin(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string username, std::string password)
{

	LOG_INFO << fmt::format("admin::login::signin username {:s} password {:s}", username, password);
	Json::Value jsrsp;

	if (username == "lains" && password == "7777") {
		jsrsp["state"] = true;
		jsrsp["msg"] = "登录成功";
	}
	else {
		jsrsp["state"] = false;
		jsrsp["msg"] = "账号或密码不正确";
	}
	auto resp = HttpResponse::newHttpJsonResponse(jsrsp);
	callback(resp);
}
void login::signout(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{

	LOG_INFO << fmt::format("signout");
	auto resp = HttpResponse::newHttpJsonResponse(true);
	callback(resp);
}
