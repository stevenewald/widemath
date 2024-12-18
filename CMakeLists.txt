cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    widemath
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    widemath_widemath
    source/widemath.cpp
)
add_library(widemath::widemath ALIAS widemath_widemath)

include(GenerateExportHeader)
generate_export_header(
    widemath_widemath
    BASE_NAME widemath
    EXPORT_FILE_NAME export/widemath/widemath_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(widemath_widemath PUBLIC WIDEMATH_STATIC_DEFINE)
endif()

set_target_properties(
    widemath_widemath PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME widemath
    OUTPUT_NAME widemath
)

target_include_directories(
    widemath_widemath ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    widemath_widemath SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(widemath_widemath PUBLIC cxx_std_23)

find_package(fmt REQUIRED)
target_link_libraries(widemath_widemath PRIVATE fmt::fmt)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT widemath_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of widemath"
  )
endif()

include(cmake/dev-mode.cmake)
