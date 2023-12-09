configure_file(config.hpp.in config.hpp)

add_library(miscutils_header INTERFACE)
set_target_properties(miscutils_header PROPERTIES EXPORT_NAME header)
target_include_directories(miscutils_header
  INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${miscutils_INSTALL_INCLUDEDIR}>)

add_library(miscutils::header ALIAS miscutils_header)

add_executable(fail fail.cpp)
target_link_libraries(fail PUBLIC -static)

add_executable(succeed succeed.cpp)
target_link_libraries(succeed PUBLIC -static)

add_executable(throttle throttle.cpp)
target_link_libraries(throttle
  PUBLIC -static
  PRIVATE
  nlohmann_json::nlohmann_json Boost::program_options)

install(TARGETS fail succeed throttle
  EXPORT miscutils_EXPORTS
  RUNTIME DESTINATION ${miscutils_INSTALL_BINDIR}
  LIBRARY DESTINATION ${miscutils_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${miscutils_INSTALL_LIBDIR})
