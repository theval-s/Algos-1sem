cmake_minimum_required(VERSION 3.15)
project(array)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/065127f1e4b46c5f14fc73cf8d323c221f9dc68e.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC GTest::gtest)
