cmake_minimum_required (VERSION 3.8)

project("XenonRecomp")

# The program as a library: everything it does, with its main as xenon_recomp_main (xenon_recomp_main.h). CodeBreaker
# links this into codebreaker.exe and calls it in-process; the XenonRecomp program below is the same library and main.
add_library(LibXenonRecomp STATIC
    "main.cpp"
    "recompiler.cpp"
    "test_recompiler.cpp"
    "recompiler_config.cpp")
target_compile_definitions(LibXenonRecomp PRIVATE XENON_RECOMP_LIBRARY)
target_include_directories(LibXenonRecomp PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../..")   # (CodeBreaker's src/: headers by their path under it)
target_link_libraries(LibXenonRecomp PUBLIC
    LibXenonAnalyse
    XenonUtils
    fmt::fmt
    tomlplusplus::tomlplusplus
    xxHash::xxhash)

add_executable(XenonRecomp "main.cpp")
target_link_libraries(XenonRecomp PRIVATE LibXenonRecomp)

foreach(target LibXenonRecomp XenonRecomp)
    target_precompile_headers(${target} PRIVATE "pch.h")

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${target} PRIVATE -Wno-switch -Wno-unused-variable -Wno-null-arithmetic)
    endif()

    # The alias form is for the compiler of the code XenonRecomp writes (Clang: the recompiled projects build with
    # clang-cl), not for the compiler XenonRecomp itself is built with: the same output whichever builds it. (Not on
    # Apple: no alias.)
    if (NOT APPLE)
        target_compile_definitions(${target} PRIVATE XENON_RECOMP_USE_ALIAS)
    endif()

    target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS)
endforeach()
