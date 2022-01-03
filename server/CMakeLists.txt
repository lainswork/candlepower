# 指定cmake的最低版本
cmake_minimum_required (VERSION 3.8)




# 服务端将会用到的库
find_package(workflow CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# 添加名为server的可执行文件。
add_executable (server "server.cpp" "server.h" )

# 链接库文件
target_link_libraries(server PRIVATE workflow)
target_link_libraries(server PRIVATE spdlog::spdlog)
target_link_libraries(server PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(server PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
# 链接 winsocket平台库
if(WIN32)
  target_link_libraries(server PRIVATE wsock32 ws2_32)
endif()

