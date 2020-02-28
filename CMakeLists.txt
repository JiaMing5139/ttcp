project(ttcp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-lboost_program_options")

add_executable(ttcp main.cpp common.h common.cpp ttcp.cpp net/InetAddress.cpp)
