# The XEX reader: CodeBreaker's one parser of the Xbox 360 executable -- the XEX2 headers, the optional-header table,
# the security info, imports, resources and TLS (xex_file.h), and the basefile's decryption and decompression
# (basefile.h). The xex engine (CMakeLists.txt here) and the recompiled-game runtime (cb/recompile/x360/runtime_lib) both
# compile it; neither has a reader of its own.
#
# Use from a CMakeLists.txt:
#   include("<CodeBreaker>/src/formats/xex/reader.cmake")
#   target_sources(<target> PRIVATE ${XEX_READER_SOURCES})
#   target_include_directories(<target> PRIVATE ${XEX_READER_INCLUDE_DIR})
# The sources compile with the including target's flags. Headers are included by their path under src/
# ("formats/xex/src/xex_file.h", "formats/xex/src/basefile.h"): XEX_READER_INCLUDE_DIR is src/ itself;
# errors are thrown as std::runtime_error / std::out_of_range.

get_filename_component(XEX_READER_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(XEX_READER_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/src/xex_file.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/basefile.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/compress/lzx.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/crypto/aes.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/crypto/keys.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/crypto/sha1.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/crypto/xex_sign.cpp")
