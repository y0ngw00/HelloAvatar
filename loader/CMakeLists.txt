cmake_minimum_required(VERSION 3.10.2)
project(loader)

SET(CMAKE_BUILD_TYPE Release CACHE STRING
	"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel.")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

link_directories(../loader/)
include_directories(../loader/)

add_compile_options(-fPIC)
add_compile_options(-std=gnu++17)
add_compile_options(-Wdeprecated-declarations)

find_package (Eigen3 3.3 REQUIRED)

file(GLOB srcs "*.h" "*.cpp")

add_library(loader ${srcs})
target_link_libraries(loader Eigen3::Eigen)
