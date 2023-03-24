
# set minimum required CMake version
cmake_minimum_required(VERSION 3.21)

# set the project name and supported languages
project(malygos LANGUAGES CXX)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# do not use C++ extensions
set(CMAKE_CXX_EXTENSIONS OFF)

# place binaries and libraries according to GNU standards
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# add subdirectory for source files
add_subdirectory(src)

# enable testing for the project
enable_testing()

# add subdirectory for test files
add_subdirectory(tests)

# Copy image assets folder to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tests)
