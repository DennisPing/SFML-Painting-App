# The minimum required version of CMake to use
cmake_minimum_required(VERSION 3.10)

project(
    App-Test        # Name of our application
    VERSION 1.0     # Version of our software
    LANGUAGES CXX)  # Language that we are using

set(CMAKE_CXX_STANDARD 17)

# Use these 2 settings for gcovr test coverage
set(CMAKE_BUILD_TYPE Profile)
set(CMAKE_CXX_FLAGS_PROFILE --coverage)

# Compile with these flags on, run the App once, then type in this command in the terminal
# gcovr -r ../../src . --html --html-details --html-details -o report.html
# Open up the report.html file to view test coverage

# Where are the include directories
include_directories("../include/")

# Find packages by using the system PATH
find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

# Our list of project source files
set(SRC_LIST
    ../src/App.cpp 
    ../src/Draw.cpp 
    ../src/Command.cpp 
    ../src/MathUtility.cpp 
)

# Our list of test source files
set(TEST_SRC_LIST
    catch_amalgamated.cpp
    MathUtilityTest.cpp
    AppTest.cpp
    DrawTest.cpp
)

# Add the source files
add_executable(${PROJECT_NAME} ${SRC_LIST} ${TEST_SRC_LIST})

# Link the SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# Add compile flag options
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)