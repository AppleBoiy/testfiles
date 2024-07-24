cmake_minimum_required(VERSION 3.28)
project(testfiles)

set(CMAKE_CXX_STANDARD 17)

include_directories(lib/lodepng)
include_directories(/opt/homebrew/Cellar/libharu/2.4.4/include)

set(SOURCE_FILES
    src/image/ImageGenerator.cpp
    src/image/ImageGenerator.h
    src/text/TextFileGenerator.cpp
    src/text/TextFileGenerator.h
    src/Generator.cpp
    src/Generator.h
    src/main.cpp
)
add_executable(testfiles ${SOURCE_FILES})

find_package(JPEG REQUIRED)
find_package(GIF REQUIRED)

add_library(lodepng STATIC lib/lodepng/lodepng.cpp)
target_link_libraries(testfiles lodepng JPEG::JPEG)

include_directories(lib/gif)

target_link_libraries(testfiles GIF::GIF /opt/homebrew/Cellar/libharu/2.4.4/lib/libhpdf.2.4.4.dylib)
