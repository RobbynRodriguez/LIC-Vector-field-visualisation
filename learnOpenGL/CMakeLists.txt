cmake_minimum_required(VERSION 3.21)
project(learnOpenGL)

set(CMAKE_CXX_STANDARD 17)

#find the required packages
find_package(OpenGL REQUIRED)
find_package(Qt5 COMPONENTS OpenGL REQUIRED)

find_package(OpenGL REQUIRED) # define OPENGL_LIBRARIES
find_package(X11 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(LIBS glfw GLU X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL pthread)

FILE(GLOB_RECURSE
        folder_source
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/*.c
        )

FILE(GLOB_RECURSE
        header_source
        ${CMAKE_SOURCE_DIR}/src/*.h
        )

include_directories(${CMAKE_SOURCE_DIR}/includes)

add_executable(main ${folder_source} ${header_source})

target_link_libraries(main ${LIBS} ${CMAKE_DL_LIBS})

#OpenGL deprecated on MacosX : remove compilation warning
#add_definitions(-DGL_SILENCE_DEPRECATION -DGLM_FORCE_SILENT_WARNINGS)