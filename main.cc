#include <drogon/drogon.h>
#include <filesystem>
int main() {

    // 输出查看exe的执行目录
    LOG_INFO << std::filesystem::current_path().string();

    // exe在很深的文件夹中，所以这里要写 ../../../config.json 发行版本改成  config.json 就行
    drogon::app().loadConfigFile("../../../config.json");

    // 启动服务器
    drogon::app().run();
    return 0;
}
