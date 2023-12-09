cmake_minimum_required(VERSION 3.12)

project(miscutils VERSION 0.1.0 LANGUAGES C CXX)

option(miscutils_BUILD_TESTING "Build the miscutils tests" ON)
set(miscutils_CXX_STANDARD 20 CACHE STRING "C++ standard for miscutils")

include(GNUInstallDirs)
set(miscutils_INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR})
set(miscutils_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
set(miscutils_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})
set(miscutils_INSTALL_CONFDIR ${miscutils_INSTALL_LIBDIR}/cmake/miscutils-${PROJECT_VERSION})

list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)
include(miscutils_deps)

add_subdirectory(miscutils)

if(miscutils_BUILD_TESTING)
  include(CTest)
  if(BUILD_TESTING)
    add_subdirectory(miscutils_testing)
  endif()
endif()

install(EXPORT miscutils_EXPORTS
  NAMESPACE miscutils::
  FILE miscutils-exports.cmake
  DESTINATION ${miscutils_INSTALL_CONFDIR})

configure_file(miscutils-config.cmake.in miscutils-config.cmake)
install(FILES ${PROJECT_BINARY_DIR}/miscutils-config.cmake
  DESTINATION ${miscutils_INSTALL_CONFDIR})
