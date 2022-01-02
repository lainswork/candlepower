#include <iostream>
#include <filesystem>
#include <workflow/WFHttpServer.h>
#include <workflow/WFMySQLServer.h>
#include <workflow/MySQLResult.h>
#include <workflow/mysql_parser.h>
#include <spdlog/spdlog.h>
#include <fmt/format.h>

#include "serverfun.h"


namespace fs = std::filesystem;
namespace cp = candlepower;
using json = nlohmann::json;
using namespace protocol;

/*
*   https://blog.csdn.net/s634772208/article/details/81155068
*   mysql8.0以上会遇到连接不上的问题，需要先执行下面语句 将密码认证规则改为mysql_native_password，这里演示中username password分别替换为账号密码
    ALTER USER username@localhost IDENTIFIED WITH mysql_native_password BY 'password';
    然后执行
    FLUSH PRIVILEGES;


*/
const char* mysql_url = "mysql://lains:990720@127.0.0.1:3306/candlepower?character_set=utf8&character_set_results=utf8";

#define _TEXT(str) u8##str

#ifndef RETRY_MAX
#define RETRY_MAX 0
#endif // RETRY_MAX

int main()
{
#ifdef WIN32
    //设置控制台为 utf8
    std::system("CHCP 65001");
#endif // WIN32

   
    json test_json = json::parse(R"(["89","100","200"])");;
  
    SPDLOG_INFO(_TEXT("{:s}"), (std::string)test_json[2]);

    return 0;





  






    SPDLOG_INFO(_TEXT("服务端启动: {:s}"),fs::current_path().generic_string());


    WFHttpServer server([](WFHttpTask *pHttpReqTask) {


        WFMySQLTask* sqlTask = WFTaskFactory::create_mysql_task(mysql_url, RETRY_MAX, [pHttpReqTask](WFMySQLTask* pSqlTask) {


            if (pSqlTask->get_state() == WFT_STATE_SUCCESS) {
                MySQLResultCursor cursor(pSqlTask->get_resp());
                auto result = fmt::format("cursor_status={:d} field_count={:d} rows_count={:d}\n",
                    cursor.get_cursor_status(), cursor.get_field_count(),
                    cursor.get_rows_count());


                pHttpReqTask->get_resp()->append_output_body(result);
            }
            else {
                
                SPDLOG_ERROR(_TEXT("WFMySQLTask 状态{:d} 错误 {:d}"), pSqlTask->get_state(), pSqlTask->get_error());
                pHttpReqTask->get_resp()->append_output_body(_TEXT("sql task erro!"));
                
            }

            
           

            });

        sqlTask->get_req()->set_query(R"(SHOW TABLES;)");

        // 将这个任务加入到任务流
        SeriesWork* series = series_of(pHttpReqTask);
        *series << sqlTask;


        
    });


    if (server.start(8889) == 0) {

        do {
            std::string cmd;
            std::getline(std::cin, cmd);
            if (cmd == "stop") {
                break;
            }
        } while (true);



        SPDLOG_INFO(_TEXT("即将中止服务.."));
        server.stop();
        SPDLOG_INFO(_TEXT("服务已中止"));
    }
    else
    {
        SPDLOG_ERROR(_TEXT("服务器启动失败"));
    }

    SPDLOG_INFO(_TEXT("进程退出"));
    return 0;
}