# The minimum required version of CMake to use
cmake_minimum_required(VERSION 3.10)

project(
    App             # Name of our application
    VERSION 1.0     # Version of our software
    LANGUAGES CXX)  # Language that we are using

set(CMAKE_CXX_STANDARD 17)

# Toggle this setting to switch between debug and release.
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Where are the include directories
include_directories("./include/")

# Find packages by using the system PATH
find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

# Our list of project source files
set(SRC_LIST
    src/App.cpp 
    # src/Draw.cpp 
    # src/Command.cpp 
    src/MathUtility.cpp 
    src/RoundedLine.cpp
    src/main.cpp 
)

# Add the source files
add_executable(${PROJECT_NAME} ${SRC_LIST})

# Link the SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# Add compile flag options
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)