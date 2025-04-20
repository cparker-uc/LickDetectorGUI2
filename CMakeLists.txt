cmake_minimum_required(VERSION 3.16)
project(LickDetectorGUI VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
find_library(FTD2XX NAMES ftd2xx FTD2XX64 PATHS third_party/ftd2xx/)
find_library(LIBMPSSE NAMES libmpsse PATHS third_party/libmpsse/build/x64/DLL/)
message(STATUS "FTD2XX library found at: ${FTD2XX}")
message(STATUS "LIBMPSSE library found at: ${LIBMPSSE}")
add_executable(LickDetectorGUI main.cpp mainwindow.cpp mpr121_controller.cpp)

target_link_libraries(LickDetectorGUI
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    ${FTD2XX}
    ${LIBMPSSE}
)
target_include_directories(LickDetectorGUI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ftd2xx/
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libmpsse/include
)
set_target_properties(LickDetectorGUI PROPERTIES
    WIN32_EXECUTABLE TRUE
)