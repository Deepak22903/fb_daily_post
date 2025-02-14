# Cmake Version
cmake_minimum_required(VERSION 3.10)

# C++ Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define project name and version
project(fb_post VERSION 1.0)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# adding all source files to SRC_FILES variable
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# define executable file and sources
add_executable(fb_post ${SRC_FILES})

# define include directory
target_include_directories(fb_post PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# nlohmann/json - Header-only
find_package(nlohmann_json REQUIRED)
target_link_libraries(fb_post PRIVATE nlohmann_json::nlohmann_json)

# libcurl
find_package(CURL REQUIRED)
target_include_directories(fb_post PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(fb_post PRIVATE ${CURL_LIBRARIES})

# cppcodec - Header-only
target_include_directories(fb_post PRIVATE /usr/include/cppcodec/)

# Poco
find_package(Poco REQUIRED COMPONENTS Foundation) # Add other components if needed
target_link_libraries(fb_post PRIVATE Poco::Foundation)

