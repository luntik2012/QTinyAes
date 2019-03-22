cmake_minimum_required(VERSION 3.10.2)

project(qtinyaes LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        REQUIRED)

file(GLOB CPP_SOURCES *.h *.cpp ../tiny-AES-c/aes.c ../tiny-AES-c/aes.h)
add_library(${PROJECT_NAME} STATIC ${CPP_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/../tiny-AES-c/)
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui)
#target_link_libraries(${PROJECT_NAME}
#    tiny-aes-c)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
