cmake_minimum_required(VERSION 3.17)

project(FaceMorphing)

#sets
set(CMAKE_CXX_STANDARD 20)

#packages
find_package(dlib REQUIRED)
find_package(OpenCV REQUIRED)

#exe
add_executable(FaceMorphing main.cpp)

#dirs
include_directories(FaceMorphing ${OpenCV_INCLUDE_DIRS})

file(
        GLOB SOURCES
        *.h
        *.cpp
)

#linking
target_link_libraries(
        FaceMorphing dlib
        dlib::dlib
)
target_link_libraries(FaceMorphing ${OpenCV_LIBS})