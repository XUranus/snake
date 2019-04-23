cmake_minimum_required(VERSION 3.12)
project(snake_client)

set(CMAKE_CXX_STANDARD 11)

add_executable(snake_client main.cpp cpp/zone.cpp header/zone.h cpp/net_service.cpp header/net_service.h header/logger.h header/gameMessage.pb.cc header/gameMessage.pb.h)

target_link_libraries(snake_client pthread curses protobuf)