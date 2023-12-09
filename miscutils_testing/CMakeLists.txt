find_package(Catch2 REQUIRED)

macro(miscutils_add_test test_name)
  set(exe_name ${test_name}_test)
  set(src_name ${exe_name}.cpp)
  add_executable(${exe_name} ${src_name})
  target_link_libraries(${exe_name} PRIVATE
    miscutils::header
    Catch2::Catch2WithMain)
  set_target_properties(${exe_name}
    PROPERTIES
    CXX_STANDARD ${miscutils_CXX_STANDARD})
  add_test(${test_name} ${exe_name})
endmacro()

add_test(NAME fail_test COMMAND fail)
set_tests_properties(fail_test PROPERTIES WILL_FAIL ON)

add_test(NAME succeed_test COMMAND succeed)
