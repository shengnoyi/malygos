include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
  unit_tests
  test_main.cpp
)
target_link_libraries(
  unit_tests
  PRIVATE
  Model
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests
  PROPERTIES
  LABELS "unit"
  DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
  240
)