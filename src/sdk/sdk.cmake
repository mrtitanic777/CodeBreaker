# sdk/sdk.cmake -- the CodeBreaker SDK (docs/SDK_DESIGN.md): include/cb, implemented in src/sdk.
#
#   cb_sdk        the area's static library (src/sdk/*.cpp); its tests join cb_tests and cb_tests_sdk like any area's
#   cbsdk         cbsdk.dll + cbsdk.lib: cb_sdk behind the C interface, exporting only the cb_* functions the headers declare
#                 (the export list is read from include/cb/*.h here, so a header and the DLL cannot disagree)
#   cb_sdk_reference   the reference generator: docs/sdk/reference from the comments of include/cb (built on request)
#
#   cmake --build build --target cbsdk                            (what it makes for itself is in build/cbsdk-made)
#   cmake --install build --component sdk --prefix <prefix>       bin/ include/cb lib/ lib/cmake/CodeBreaker share/codebreaker
#
# A program uses the installed SDK with find_package(CodeBreaker) and the target CodeBreaker::cbsdk (samples/).
set(CB_SDK_INCLUDE "${CB_NATIVE_ROOT}/../include")
get_filename_component(CB_SDK_INCLUDE "${CB_SDK_INCLUDE}" ABSOLUTE)
get_filename_component(CB_SDK_CHECKOUT "${CB_NATIVE_ROOT}/.." ABSOLUTE)

file(STRINGS "${CB_SDK_INCLUDE}/cb/version.h" cb_sdk_version_lines REGEX "^#define CB_SDK_VERSION_(MAJOR|MINOR|PATCH) ")
foreach(line IN LISTS cb_sdk_version_lines)
  if(line MATCHES "CB_SDK_VERSION_(MAJOR|MINOR|PATCH) ([0-9]+)")
    set(CB_SDK_VERSION_${CMAKE_MATCH_1} ${CMAKE_MATCH_2})
  endif()
endforeach()
set(CB_SDK_VERSION "${CB_SDK_VERSION_MAJOR}.${CB_SDK_VERSION_MINOR}.${CB_SDK_VERSION_PATCH}")

# (cb_area(sdk) is called by src/CMakeLists.txt, which includes this file after it)
target_include_directories(cb_sdk PUBLIC "${CB_SDK_INCLUDE}")
target_compile_definitions(cb_sdk PUBLIC CB_SDK_STATIC)   # (the declarations carry no dllimport inside the library and its tests)
target_link_libraries(cb_sdk PUBLIC cb_core cb_base cb_formats cb_source cb_decompile cb_decompiler cb_engine)

# The functions the DLL exports: every `CB_API ... cb_name(` of the public headers.
file(GLOB CB_SDK_HEADERS CONFIGURE_DEPENDS "${CB_SDK_INCLUDE}/cb/*.h" "${CB_SDK_INCLUDE}/cb/*.hpp")
set(CB_SDK_EXPORTS "")
foreach(header IN LISTS CB_SDK_HEADERS)
  file(STRINGS "${header}" lines REGEX "^CB_API ")
  foreach(line IN LISTS lines)
    if(line MATCHES "(cb_[a-z0-9_]+)\\(")
      list(APPEND CB_SDK_EXPORTS "${CMAKE_MATCH_1}")
    endif()
  endforeach()
endforeach()
list(SORT CB_SDK_EXPORTS)
list(JOIN CB_SDK_EXPORTS "\n    " cb_sdk_def_lines)
file(CONFIGURE OUTPUT "${CMAKE_BINARY_DIR}/cbsdk-made/cbsdk.def" CONTENT "LIBRARY cbsdk\nEXPORTS\n    ${cb_sdk_def_lines}\n" @ONLY)
list(JOIN CB_SDK_EXPORTS ";\n    " cb_sdk_map_lines)
file(CONFIGURE OUTPUT "${CMAKE_BINARY_DIR}/cbsdk-made/cbsdk.map" CONTENT "{\n  global:\n    ${cb_sdk_map_lines};\n  local: *;\n};\n" @ONLY)

if(WIN32)
  add_library(cbsdk SHARED "${CMAKE_BINARY_DIR}/cbsdk-made/cbsdk.def" "${CMAKE_CURRENT_LIST_DIR}/cbsdk.rc")
  target_include_directories(cbsdk PRIVATE "${CB_SDK_INCLUDE}")   # (the resource compiler reads cb/version.h)
else()
  # (ELF: the version script keeps the cb_* names alone visible; -u pulls each one's object out of cb_sdk)
  file(WRITE "${CMAKE_BINARY_DIR}/cbsdk-made/cbsdk_empty.cpp" "// cbsdk: its code is cb_sdk's (sdk/sdk.cmake)\n")
  add_library(cbsdk SHARED "${CMAKE_BINARY_DIR}/cbsdk-made/cbsdk_empty.cpp")
  target_link_options(cbsdk PRIVATE "-Wl,--version-script=${CMAKE_BINARY_DIR}/cbsdk-made/cbsdk.map")
  foreach(name IN LISTS CB_SDK_EXPORTS)
    target_link_options(cbsdk PRIVATE "-Wl,-u,${name}")
  endforeach()
endif()
set_target_properties(cbsdk PROPERTIES VERSION "${CB_SDK_VERSION}" SOVERSION "${CB_SDK_VERSION_MAJOR}"
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_link_libraries(cbsdk PRIVATE cb_sdk)
add_dependencies(cbsdk decompiler)   # (the program the decompile calls run, installed beside it)
cb_copy_prebuilt_dlls(cbsdk)

# The reference generator: tools/sdk/reference.cmd runs it.
# (reference.cpp needs only core: compiled into the tool again rather than linking cb_sdk and every engine behind it)
add_executable(cb_sdk_reference EXCLUDE_FROM_ALL "${CMAKE_CURRENT_LIST_DIR}/reference_main.cpp" "${CMAKE_CURRENT_LIST_DIR}/reference.cpp")
cb_apply_flags(cb_sdk_reference)
target_link_libraries(cb_sdk_reference PRIVATE cb_core)

# ---- the installed SDK -----------------------------------------------------------------------------------------------------
install(TARGETS cbsdk RUNTIME DESTINATION bin COMPONENT sdk LIBRARY DESTINATION lib COMPONENT sdk ARCHIVE DESTINATION lib COMPONENT sdk)
install(FILES ${CB_SDK_HEADERS} DESTINATION include/cb COMPONENT sdk)
install(PROGRAMS "$<TARGET_FILE:decompiler>" DESTINATION bin COMPONENT sdk)
if(CB_PREBUILT_LIBS)
  install(FILES ${CB_PREBUILT_LIBS} DESTINATION bin COMPONENT sdk)
endif()
# the data root (cb_init): the configuration and the processors' compiled descriptions
install(DIRECTORY "${CB_SDK_CHECKOUT}/config/" DESTINATION share/codebreaker/config COMPONENT sdk
  PATTERN "app.json" EXCLUDE PATTERN "app.recent.json" EXCLUDE PATTERN "attached_tools.json" EXCLUDE)
install(DIRECTORY "${CB_NATIVE_ROOT}/decompile/engine/languages/" DESTINATION share/codebreaker/languages COMPONENT sdk
  FILES_MATCHING PATTERN "*.sla" PATTERN "*.ldefs" PATTERN "*.pspec" PATTERN "*.cspec" PATTERN "src" EXCLUDE)
install(DIRECTORY "${CB_SDK_CHECKOUT}/docs/sdk/reference/" DESTINATION share/doc/codebreaker/reference COMPONENT sdk)
# find_package(CodeBreaker): the package's files, with the library's file names as this build made them
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/cbsdk-made/CodeBreakerConfig.cmake" INPUT "${CMAKE_CURRENT_LIST_DIR}/CodeBreakerConfig.cmake.in")
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_BINARY_DIR}/cbsdk-made/CodeBreakerConfigVersion.cmake" VERSION "${CB_SDK_VERSION}"
  COMPATIBILITY SameMinorVersion)   # (before 1.0 a minor version may change what an earlier one had: docs/SDK_DESIGN.md)
install(FILES "${CMAKE_BINARY_DIR}/cbsdk-made/CodeBreakerConfig.cmake" "${CMAKE_BINARY_DIR}/cbsdk-made/CodeBreakerConfigVersion.cmake"
  DESTINATION lib/cmake/CodeBreaker COMPONENT sdk)

# The SDK's speed against the engines it wraps (cb_sdk_bench <checkout> <state>): each call timed against the internal call. Built on request.
add_executable(cb_sdk_bench EXCLUDE_FROM_ALL "${CMAKE_CURRENT_LIST_DIR}/bench_main.cpp")
cb_apply_flags(cb_sdk_bench)
target_link_libraries(cb_sdk_bench PRIVATE cb_sdk cb_core)
cb_copy_prebuilt_dlls(cb_sdk_bench)
add_dependencies(cb_sdk_bench decompiler)
