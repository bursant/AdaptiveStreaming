cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

find_package(SDL REQUIRED)
find_package(LibAV REQUIRED)

include_directories(.)

file(GLOB_RECURSE client *.cpp)

add_executable(client ${client})
target_link_libraries(client dash ${SDL_LIBRARY} ${LIBAV_LIBRARIES})