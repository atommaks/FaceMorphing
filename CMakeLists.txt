cmake_minimum_required(VERSION 3.17)

project(FaceMorphing)

#sets
set(CMAKE_CXX_STANDARD 14)

#packages
find_package(dlib REQUIRED)
find_package(OpenCV REQUIRED)

#exe
add_executable(FaceMorphing main.cpp)

#dirs
include_directories(FaceMorhing ${OpenCV_INCLUDE_DIRS})

#linking
target_link_libraries(
        FaceMorphing dlib
        dlib::dlib
)
target_link_libraries(FaceMorphing ${OpenCV_LIBS})