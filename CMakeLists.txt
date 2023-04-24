# CMakeList.txt: socket 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("socket")

# 将源代码添加到此项目的可执行文件。
add_executable (socket "src/remove_zombie.cpp")

if (CMAKE_SYSTEM_NAME MATCHES "Windows")          # for Windows
	target_link_libraries(socket "ws2_32.lib")
endif()

#execute_process(COMMAND ls .. 
#               OUTPUT_VARIABLE ls_ret)#执行外部命令

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET socket PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
