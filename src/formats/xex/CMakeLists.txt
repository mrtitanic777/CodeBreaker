# cb/formats/xex: CodeBreaker's Xbox 360 executable engine (MIT -- see LICENSE). It reads a .xex, writes its basefile
# (the PowerPC image inside, decrypted and decompressed), and converts, patches and re-signs it. Built as the library
# cb_xex, part of src's one build (added from cb/CMakeLists.txt): cb::formats::xex parses with it, writes basefiles
# with it, and runs its command line in-process as `codebreaker xex <command>` (src/command.h). Compiled as it is, under
# its own flags (not /W4 /WX).
#
# reader.cmake names the parser alone (the reader, the basefile's decryption and decompression) for the recompiled-game
# runtime, which compiles it with clang-cl (cb/recompile/x360).
include("${CMAKE_CURRENT_LIST_DIR}/reader.cmake")
file(GLOB CB_XEX_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/crypto/*.cpp"
     "${CMAKE_CURRENT_LIST_DIR}/src/compress/*.cpp")
add_library(cb_xex STATIC ${CB_XEX_SOURCES})
set_target_properties(cb_xex PROPERTIES CXX_STANDARD 20 MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(cb_xex PUBLIC "${XEX_READER_INCLUDE_DIR}")
target_compile_options(cb_xex PRIVATE ${CB_FLAGS_LIB})
if(MSVC)
  target_compile_definitions(cb_xex PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
if(CB_ASAN)
  target_compile_options(cb_xex PRIVATE /fsanitize=address /Zi)
endif()

# The LZX encoder's and the signer's standalone harnesses (tools/), built on request.
foreach(t test_lzxenc test_sign)
  add_executable(cb_xex_${t} EXCLUDE_FROM_ALL "${CMAKE_CURRENT_LIST_DIR}/tools/${t}.cpp")
  target_link_libraries(cb_xex_${t} PRIVATE cb_xex)
  set_target_properties(cb_xex_${t} PROPERTIES CXX_STANDARD 20 MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  target_compile_options(cb_xex_${t} PRIVATE ${CB_FLAGS_LIB})
endforeach()
