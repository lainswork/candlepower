#include <iostream>
#include <filesystem>
#include <workflow/WFHttpServer.h>

#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

int main()
{
    SPDLOG_INFO("服务端启动: {:s}",fs::current_path().generic_string());


    WFHttpServer server([](WFHttpTask *task) {
        task->get_resp()->append_output_body("<html>Hello World!</html>");
    });


    if (server.start(8889) == 0) {

        do {
            std::string cmd;
            std::getline(std::cin, cmd);
            if (cmd == "stop") {
                break;
            }
        } while (true);



        SPDLOG_INFO("即将中止服务..");
        server.stop();
        SPDLOG_INFO("服务已中止");
    }
    else
    {
        SPDLOG_ERROR("服务器启动失败");
    }

    SPDLOG_INFO("进程退出");
    return 0;
}