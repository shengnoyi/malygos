# Add your source files here
set(SOURCES main.cpp)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(EIGEN3_INCLUDE_DIR)

# Create a static library from tgaimage.cpp and tgaimage.h
add_library(TGAImage STATIC tgaimage.cpp tgaimage.h)

# Create a library target for the geometry header-only library
add_library(Geometry INTERFACE)
target_include_directories(Geometry INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Geometry INTERFACE Eigen3::Eigen)

# Create a static library from model.cpp and model.h
add_library(Model STATIC model.cpp model.h)

# Link the Model library with the Geometry library
target_link_libraries(Model Geometry)

# Create a static library from renderer.cpp and renderer.h
add_library(Renderer STATIC renderer.cpp renderer.h)

target_link_libraries(Renderer Geometry)

add_executable(${PROJECT_NAME} ${SOURCES})

# Link the TGAImage, Model, and Geometry libraries
target_link_libraries(${PROJECT_NAME} TGAImage Model Renderer)

target_include_directories(malygos PUBLIC "${PROJECT_BINARY_DIR}")
