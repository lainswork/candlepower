# 指定cmake的最低版本
cmake_minimum_required (VERSION 3.8)
# 指定vcpkg的路径 $ENV{Vcpkg} 就是系统环境变量 ， 可以替换为 vcpkg.cmake的绝对路径
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")
# 禁用一些警告
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -_CRT_SECURE_NO_WARNINGS=1")

# 声明项目名称 和使用的语言
project ("candlepower" LANGUAGES C CXX)


# 启用 C++17
set(CMAKE_CXX_STANDARD 17)


add_subdirectory(server)
add_subdirectory(reseller)
add_subdirectory(client)
add_subdirectory(admin)