cmake_minimum_required(VERSION 3.12)
project(snake_server)

set(CMAKE_CXX_STANDARD 11)

add_executable(snake_server main.cpp cpp/snake.cpp header/snake.h cpp/body_node.cpp header/body_node.h cpp/server.cpp header/server.h cpp/zone.cpp header/zone.h cpp/timer.cpp header/timer.h cpp/food.cpp header/food.h header/gameMessage.pb.h header/gameMessage.pb.cc)

TARGET_LINK_LIBRARIES(snake_server pthread curses protobuf)