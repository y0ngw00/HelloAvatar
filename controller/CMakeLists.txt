cmake_minimum_required(VERSION 3.10.2)
project(con)

SET(CMAKE_BUILD_TYPE Release CACHE STRING
	"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel.")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

link_directories(../loader/)
include_directories(../loader/)

add_compile_options(-fPIC)
add_compile_options(-std=gnu++17)
add_compile_options(-Wdeprecated-declarations)


# find_package(DART REQUIRED COMPONENTS collision-bullet CONFIG)
find_package(PythonLibs REQUIRED)
find_package(pybind11 REQUIRED)
find_package (Eigen3 3.3 REQUIRED)

include_directories(${DART_INCLUDE_DIRS})
# include_directories(${PYTHON_INCLUDE_DIR})
file(GLOB srcs "*.h" "*.cpp")

add_library(con ${srcs})
target_link_libraries(con ${DART_LIBRARIES} loader Eigen3::Eigen)
