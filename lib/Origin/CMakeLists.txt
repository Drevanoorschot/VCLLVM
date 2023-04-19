include(FetchContent)

FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP true)
FetchContent_MakeAvailable(json)

add_library(OriginProvider STATIC
        OriginProvider.cpp
        ShortPositionDeriver.cpp
        ContextDeriver.cpp
        PreferredNameDeriver.cpp)

target_link_libraries(OriginProvider nlohmann_json::nlohmann_json)
target_include_directories(OriginProvider PRIVATE
        "${CMAKE_SOURCE_DIR}/include")