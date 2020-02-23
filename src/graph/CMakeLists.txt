cmake_minimum_required(VERSION 3.5)
project(graphs)

set(CMAKE_CXX_STANDARD 17)

include_directories(

    )

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Boost REQUIRED)


add_executable(graph graph.hpp graph.cpp parsing.hpp)



target_link_libraries(graph PRIVATE nlohmann_json::nlohmann_json)
