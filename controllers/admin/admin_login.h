#pragma once
#include <drogon/HttpController.h>
using namespace drogon;
namespace admin
{
class login:public drogon::HttpController<login>
{
  public:
    METHOD_LIST_BEGIN

    METHOD_ADD(login::signin, "/signin?username={1}&password={2}", Post);
    METHOD_ADD(login::signout, "/signout?", Post);

    METHOD_LIST_END

     void signin(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback, std::string username,std::string password);
     void signout(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
};
}
