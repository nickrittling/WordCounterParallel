cmake_minimum_required (VERSION 3.4)

set(CMAKE_BUILDS_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

project(WordCounterParallel)

#include(FetchContent)
#FetchContent_Declare(
#  cli11
#  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
#  GIT_TAG        v2.2.0
#)

#FetchContent_MakeAvailable(cli11)

#FetchContent_MakeAvailable(cli11)
#find_package(OpenSSL REQUIRED)


#add_subdirectory(external/CLI11)
#add_subdirectory(external/sha256)
#set(SOURCES file.cpp file2.cpp ${YOUR_DIRECTORY}/file1.h ${YOUR_DIRECTORY}/file2.h)
set(SOURCES WordCounter.cpp bloom.h CLI11.hpp)
add_executable(
WordCounterParallel
WordCounterParallel.cpp
)
#target_link_libraries(bloom CLI11)
#target_link_libraries(bloom sha256)
#target_link_libraries(WordCounterParallel OpenSSL::SSL OpenSSL::Crypto)


