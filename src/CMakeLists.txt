include(FetchContent)

# adding all source files to SRC_FILES variable
file(GLOB_RECURSE SRC_FILES ./*.cpp)

# define executable file and sources
add_executable(fb_post ${SRC_FILES})

# define include directory
target_include_directories(fb_post PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# nlohmann/json
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
  message(STATUS "nlohmann_json not found. Downloading...")
  FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
    )
  FetchContent_MakeAvailable(json)
endif()
target_link_libraries(fb_post PRIVATE nlohmann_json::nlohmann_json)

# libcurl
find_package(CURL QUIET)
if(NOT CURL_FOUND)
  message(STATUS "CURL not found. Downloading...")
  FetchContent_Declare(
        curl
        GIT_REPOSITORY https://github.com/curl/curl.git
        GIT_TAG curl-8_4_0
    )
  set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
  set(BUILD_TESTING OFF CACHE INTERNAL "")
  FetchContent_MakeAvailable(curl)
endif()
target_include_directories(fb_post PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(fb_post PRIVATE ${CURL_LIBRARIES})

# cppcodec
find_package(cppcodec QUIET)
if(NOT EXISTS "/usr/include/cppcodec/")
  message(STATUS "cppcodec not found. Downloading...")
  FetchContent_Declare(
        cppcodec
        GIT_REPOSITORY https://github.com/tplgy/cppcodec.git
        GIT_TAG v0.2
    )
  FetchContent_MakeAvailable(cppcodec)
  target_include_directories(fb_post PRIVATE ${cppcodec_SOURCE_DIR})
else()
  target_include_directories(fb_post PRIVATE /usr/include/cppcodec/)
endif()

# Poco
find_package(Poco COMPONENTS Foundation QUIET)
if(NOT Poco_FOUND)
  message(STATUS "Poco not found. Downloading...")
  FetchContent_Declare(
        poco
        GIT_REPOSITORY https://github.com/pocoproject/poco.git
        GIT_TAG poco-1.12.4-release
    )
  set(ENABLE_MONGODB OFF CACHE INTERNAL "")
  set(ENABLE_DATA_SQLITE OFF CACHE INTERNAL "")
  set(ENABLE_REDIS OFF CACHE INTERNAL "")
  set(ENABLE_PDF OFF CACHE INTERNAL "")
  set(ENABLE_TESTS OFF CACHE INTERNAL "")
  FetchContent_MakeAvailable(poco)
endif()
target_link_libraries(fb_post PRIVATE Poco::Foundation)

