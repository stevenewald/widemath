project(WidemathBenchmarks LANGUAGES CXX)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
# ---- Dependencies ----

find_package(benchmark REQUIRED)

# ---- Benchmarks ----
add_executable(widemath_benchmarks
  source/main.cpp
  source/uint128.cpp
)

target_include_directories(
  widemath_benchmarks ${warning_guard}
  PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_link_libraries(
  widemath_benchmarks PRIVATE
  widemath_widemath
  benchmark::benchmark
)

# Enable lto for release builds
include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT error)
if(lto_supported)
  set_target_properties(widemath_benchmarks PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
    message(WARNING "LTO is not supported: ${error}")
endif()

target_compile_features(widemath_benchmarks PRIVATE cxx_std_23)

