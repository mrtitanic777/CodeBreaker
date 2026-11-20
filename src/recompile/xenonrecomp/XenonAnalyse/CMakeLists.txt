# cmake_minimum_required (VERSION 3.16)

project("XenonAnalyse")

add_library(LibXenonAnalyse "function.cpp")
target_include_directories(LibXenonAnalyse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(LibXenonAnalyse PUBLIC XenonUtils)

# The program as a library: its main as xenon_analyse_main (xenon_analyse_main.h). CodeBreaker links this into
# codebreaker.exe and calls it in-process; the XenonAnalyse program below is the same library and main.
add_library(LibXenonAnalyseMain STATIC "main.cpp")
target_compile_definitions(LibXenonAnalyseMain PRIVATE XENON_ANALYSE_LIBRARY)
target_include_directories(LibXenonAnalyseMain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(LibXenonAnalyseMain PUBLIC LibXenonAnalyse XenonUtils fmt::fmt)

add_executable(XenonAnalyse "main.cpp")
target_link_libraries(XenonAnalyse PRIVATE LibXenonAnalyse XenonUtils fmt::fmt)
