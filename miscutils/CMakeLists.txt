configure_file(config.hpp.in config.hpp)

add_library(miscutils_header INTERFACE)
set_target_properties(miscutils_header PROPERTIES EXPORT_NAME header)
target_include_directories(miscutils_header
  INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${miscutils_INSTALL_INCLUDEDIR}>)

add_library(miscutils::header ALIAS miscutils_header)
  
install(TARGETS miscutils_header
  EXPORT miscutils_EXPORTS
  RUNTIME DESTINATION ${miscutils_INSTALL_BINDIR}
  LIBRARY DESTINATION ${miscutils_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${miscutils_INSTALL_LIBDIR})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
  DESTINATION ${miscutils_INSTALL_INCLUDEDIR}/miscutils)
