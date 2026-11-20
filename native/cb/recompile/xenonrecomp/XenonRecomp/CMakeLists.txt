cmake_minimum_required (VERSION 3.8)

project("XenonRecomp")

add_executable(XenonRecomp 
    "main.cpp" 
    "recompiler.cpp"
    "test_recompiler.cpp" 
    "recompiler_config.cpp")

target_precompile_headers(XenonRecomp PUBLIC "pch.h")

target_link_libraries(XenonRecomp PRIVATE
    LibXenonAnalyse 
    XenonUtils 
    fmt::fmt
    tomlplusplus::tomlplusplus 
    xxHash::xxhash)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(XenonRecomp PRIVATE -Wno-switch -Wno-unused-variable -Wno-null-arithmetic)
endif()

# The alias form is for the compiler of the code XenonRecomp writes (Clang: the recompiled projects build with clang-cl),
# not for the compiler XenonRecomp itself is built with: the same output whichever builds it. (Not on Apple: no alias.)
if (NOT APPLE)
    target_compile_definitions(XenonRecomp PRIVATE XENON_RECOMP_USE_ALIAS)
endif()

target_compile_definitions(XenonRecomp PRIVATE _CRT_SECURE_NO_WARNINGS)
