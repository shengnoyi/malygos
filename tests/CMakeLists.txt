include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(
  unit_tests
  test_model.cpp
  test_renderer.cpp
)

target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})  # Add this line

target_link_libraries(
  unit_tests
  PRIVATE
  Model
  Renderer
  TGAImage
  gtest_main
  Eigen3::Eigen
)

include(GoogleTest)
gtest_discover_tests(unit_tests
  PROPERTIES
  LABELS "unit"
  DISCOVERY_TIMEOUT 240
)
