# adding all source files to SRC_FILES variable
file(GLOB_RECURSE SRC_FILES ./*.cpp)

# define executable file and sources
add_executable(fb_post ${SRC_FILES})


# define include directory
target_include_directories(fb_post PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

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
find_package(Poco REQUIRED COMPONENTS Foundation)
target_link_libraries(fb_post PRIVATE Poco::Foundation)

# # dotenv
# find_package(laserpants_dotenv)
# target_link_libraries(fb_post PRIVATE laserpants::dotenv)

