cmake_minimum_required(VERSION 3.19)
project(aps_os_realtime)

set(CMAKE_CXX_STANDARD 14)

add_executable(aps_os_realtime os.cpp task.cpp resource.cpp events.cpp sys.h rtos_api.h global.cpp defs.h test.cpp)