cmake_minimum_required(VERSION 3.14)

project(widemathTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(widemath REQUIRED)
  enable_testing()
endif()

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(widemath_test 
	source/widemath_test.cpp
	source/uint128_test.cpp)
target_link_libraries(
    widemath_test PRIVATE
    widemath::widemath
    Catch2::Catch2WithMain
)
target_compile_features(widemath_test PRIVATE cxx_std_23)

catch_discover_tests(widemath_test)

# ---- End-of-file commands ----

add_folders(Test)
