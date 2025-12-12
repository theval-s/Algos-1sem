cmake_minimum_required(VERSION 3.30)
project(1_hamurabi)

set(CMAKE_CXX_STANDARD 23)

add_library(HamurabiGame)
target_sources(HamurabiGame PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
        HamurabiGame.cppm
)

add_executable(1_hamurabi main.cpp)

target_link_libraries(1_hamurabi PRIVATE HamurabiGame)
