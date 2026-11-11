# native/cb: CodeBreaker's idiomatic C++ tree (docs/CPP_DESIGN.md). One static library per area, one test
# executable for all of them. Adding an area = one `cb_area(<dir>)` line below; its `*_test.cpp` files join
# cb_tests automatically.
cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0091 NEW)        # MSVC_RUNTIME_LIBRARY is honoured (static CRT, as the other libraries)

get_filename_component(CB_NATIVE_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(CB_THIRD_PARTY "${CB_NATIVE_ROOT}/third_party")

# The flags of docs/CPP_DESIGN.md 3.1, applied to every cb_* target.
function(cb_apply_flags target)
  target_compile_features(${target} PUBLIC cxx_std_20)
  set_target_properties(${target} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  target_include_directories(${target} PUBLIC "${CB_NATIVE_ROOT}" "${CB_THIRD_PARTY}")
  target_compile_definitions(${target} PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN UNICODE _UNICODE)
  target_compile_options(${target} PRIVATE /permissive- /utf-8 /EHsc /W4 /WX /Zc:__cplusplus /Zc:char8_t- /bigobj)
  if(CB_ASAN)
    target_compile_options(${target} PRIVATE /fsanitize=address /Zi)
    target_link_options(${target} PUBLIC /INCREMENTAL:NO)
  endif()
endfunction()

# cb_area(<dir>): static library cb_<dir> from <dir>/*.cpp (not *_test.cpp, not *_main.cpp); the area's tests
# are collected for cb_tests.
set_property(GLOBAL PROPERTY CB_TEST_SOURCES "")
function(cb_area dir)
  file(GLOB sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/${dir}/*.cpp")
  file(GLOB tests CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/${dir}/*_test.cpp")
  list(FILTER sources EXCLUDE REGEX "(_test|_main)\\.cpp$")
  add_library(cb_${dir} STATIC ${sources})
  cb_apply_flags(cb_${dir})
  get_property(all_tests GLOBAL PROPERTY CB_TEST_SOURCES)
  set_property(GLOBAL PROPERTY CB_TEST_SOURCES ${all_tests} ${tests})
endfunction()

cb_area(core)
target_link_libraries(cb_core PUBLIC bcrypt)

# ---- the test executable: every area's tests, compiled straight into it (a static library would let the
# linker drop test objects nothing references) -------------------------------------------------------------
get_property(CB_ALL_TESTS GLOBAL PROPERTY CB_TEST_SOURCES)
add_executable(cb_tests core/testing_main.cpp ${CB_ALL_TESTS} cb.manifest)
cb_apply_flags(cb_tests)
target_compile_definitions(cb_tests PRIVATE CB_SOURCE_DIR="${CB_NATIVE_ROOT}")
target_link_libraries(cb_tests PRIVATE cb_core)
set_source_files_properties(core/testing.cpp PROPERTIES COMPILE_DEFINITIONS CB_SOURCE_DIR="${CB_NATIVE_ROOT}")

enable_testing()
add_test(NAME cb_tests COMMAND cb_tests)
