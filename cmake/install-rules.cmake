if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/widemath-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package widemath)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT widemath_Development
)

install(
    TARGETS widemath_widemath
    EXPORT widemathTargets
    RUNTIME #
    COMPONENT widemath_Runtime
    LIBRARY #
    COMPONENT widemath_Runtime
    NAMELINK_COMPONENT widemath_Development
    ARCHIVE #
    COMPONENT widemath_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    widemath_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE widemath_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(widemath_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${widemath_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT widemath_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${widemath_INSTALL_CMAKEDIR}"
    COMPONENT widemath_Development
)

install(
    EXPORT widemathTargets
    NAMESPACE widemath::
    DESTINATION "${widemath_INSTALL_CMAKEDIR}"
    COMPONENT widemath_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
