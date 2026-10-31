# CodeBreaker's file-format readers in C++: formats.dll, a C interface (cbformats.h) loaded by
# codebreaker/formats/native.py through ctypes.  The readers give the very facts the Python
# readers of codebreaker/formats/ give; `ctest` checks the library on its own, and
# native/formats/gate.py checks it against the Python readers over a corpus.
#
#   (in a vcvars64 shell)
#   cmake -S native/formats -B native/formats/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/formats/build
#   ctest --test-dir native/formats/build
cmake_minimum_required(VERSION 3.16)
project(codebreaker_formats CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FORMATS_SOURCES
  api.cc              # cbf_call: a question to its reader, the answer back
  core/bytes.cc       # bytes read with Python's rules (slices, unpack_from, find)
  core/text.cc        # Python's decoders (latin1, utf-16-le with replace, ascii)
  core/pyvalue.cc     # the values that cross the interface (a pickle subset)
  core/files.cc       # files read by path
  pe/pe.cc            # PE (formats/pe.py)
  pe/questions.cc
  ne/ne.cc            # NE, the Windows 1.x boot image (formats/ne.py)
  ne/questions.cc
  le/le.cc            # LE and the W3/W4 containers (formats/le.py)
  le/image.cc         #   an image taken apart and written back
  le/questions.cc
  dos/dos.cc          # real-mode programs: MZ, .COM, drivers, IO.SYS, EXEPACK (formats/dos.py)
  dos/questions.cc
)

add_library(formats SHARED ${FORMATS_SOURCES})
target_include_directories(formats PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
if(MSVC)
  target_compile_options(formats PRIVATE /EHsc /O2 /W3 /utf-8)
  target_compile_definitions(formats PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
  set_property(TARGET formats PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")   # no VC runtime DLL needed
endif()
set_target_properties(formats PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
                                         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# the library on its own: hand-made images through the C interface
enable_testing()
add_executable(formats_selftest tests/selftest.cc)
target_include_directories(formats_selftest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(formats_selftest PRIVATE formats)
if(MSVC)
  target_compile_options(formats_selftest PRIVATE /EHsc /W3 /utf-8)
  target_compile_definitions(formats_selftest PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
  set_property(TARGET formats_selftest PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()
set_target_properties(formats_selftest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_test(NAME formats_selftest COMMAND formats_selftest)
