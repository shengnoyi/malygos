# Add your source files here
set(SOURCES main.cpp)

# Create a static library from tgaimage.cpp and tgaimage.h
add_library(TGAImage STATIC tgaimage.cpp tgaimage.h)

# Create a library target for the geometry header-only library
add_library(Geometry INTERFACE)
target_include_directories(Geometry INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Create a static library from model.cpp and model.h
add_library(Model STATIC model.cpp model.h)

# Create a static library from renderer.cpp and renderer.h
add_library(Renderer STATIC renderer.cpp renderer.h)

# Link the Model library with the Geometry library
target_link_libraries(Model Geometry)

add_executable(${PROJECT_NAME} ${SOURCES})

# Link the TGAImage, Model, and Geometry libraries
target_link_libraries(${PROJECT_NAME} TGAImage Model Geometry Renderer)

target_include_directories(malygos PUBLIC "${PROJECT_BINARY_DIR}")
