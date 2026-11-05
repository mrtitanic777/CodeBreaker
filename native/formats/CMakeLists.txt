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
project(codebreaker_formats C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FORMATS_SOURCES
  api.cc              # cbf_call: a question to its reader, the answer back
  core/bytes.cc       # bytes read with Python's rules (slices, unpack_from, find)
  core/text.cc        # Python's decoders (latin1, utf-16-le with replace, ascii)
  core/pyvalue.cc     # the values that cross the interface (a pickle subset)
  core/files.cc       # files read by path
  core/uc.cc          # Unicorn, by the DLL Python's unicorn package carries
  core/cs32.cc        # Capstone (x86-32), by the DLL Python's capstone package carries
  pe/pe.cc            # PE (formats/pe.py)
  pe/questions.cc
  ne/ne.cc            # NE, the Windows 1.x boot image (formats/ne.py)
  ne/questions.cc
  le/le.cc            # LE and the W3/W4 containers (formats/le.py)
  le/image.cc         #   an image taken apart and written back
  le/questions.cc
  dos/dos.cc          # real-mode programs: MZ, .COM, drivers, IO.SYS, EXEPACK (formats/dos.py)
  dos/unpack.cc       #   a packed program's unpacker run in the emulator (emulate/unpack.py)
  dos/questions.cc
  omf/omf.cc          # OMF-86 objects and libraries (formats/omf.py)
  omf/questions.cc
  xex/questions.cc    # the Xbox 360 executable's header (formats/xex.py), by native/xex's reader
  core/codecs.cc      # cp437, utf-16-be, utf-8 with replace
  detect/detect.cc    # what kind of file a file is (formats/detect.py)
  zx/zx.cc            # ZX Spectrum snapshots and tapes (formats/zx.py)
  lzx/lzx.cc          # LZX (LZXD) decoding (formats/lzx.py)
  cab/cab.cc          # Microsoft cabinets: read, decompress, extract, write (formats/cab.py)
  media/media.cc      # FAT, ISO-9660, partitions, El Torito, put (formats/media.py)
  win/fonts.cc        # raster fonts (formats/fonts.py)
  win/resources.cc    # the resources of a Windows module, decoded (formats/resources.py)
  win/powerpc_image.cc # what a PowerPC PE image states about its code (formats/powerpc_image.py)
  win/pe_functions.cc # the 32-bit function carve over Capstone (formats/pe_functions.py)
  win/inspect.cc      # strings, hex listing, categorize (formats/inspect.py)
  win/xex_edit.cc     # an Xbox 360 executable's privilege bits (formats/xex_edit.py)
)

# zlib 1.3.1 (MSZIP): the copy native/decompiler carries, and crc32() (core/zcrc32.c)
set(ZLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../decompiler/core/zlib)
file(GLOB ZLIB_SOURCES ${ZLIB_DIR}/*.c)
add_library(cbf_zlib STATIC ${ZLIB_SOURCES} core/zcrc32.c)
target_include_directories(cbf_zlib PUBLIC ${ZLIB_DIR})
if(MSVC)
  target_compile_options(cbf_zlib PRIVATE /O2 /W0)
  target_compile_definitions(cbf_zlib PRIVATE _CRT_SECURE_NO_WARNINGS)
  set_property(TARGET cbf_zlib PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

# Unicorn's headers, from the Python package that carries its DLL (nothing of it is linked)
find_package(Python3 COMPONENTS Interpreter REQUIRED)
if(NOT UNICORN_DIR)
  execute_process(COMMAND ${Python3_EXECUTABLE} -c "import unicorn, os; print(os.path.dirname(unicorn.__file__))"
                  OUTPUT_VARIABLE UNICORN_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

# Capstone's headers, from the Python package that carries its DLL (the same version; nothing of it is linked)
if(NOT CAPSTONE_INCLUDE)
  execute_process(COMMAND ${Python3_EXECUTABLE} -c "import capstone, os; print(os.path.join(os.path.dirname(capstone.__file__), 'include'))"
                  OUTPUT_VARIABLE CAPSTONE_INCLUDE OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

# CodeBreaker's one XEX parser (native/xex), compiled in as its other users do
include(${CMAKE_CURRENT_SOURCE_DIR}/../xex/reader.cmake)

add_library(formats SHARED ${FORMATS_SOURCES} ${XEX_READER_SOURCES})
target_include_directories(formats PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${XEX_READER_INCLUDE_DIR} ${UNICORN_DIR}/include ${CAPSTONE_INCLUDE})
target_link_libraries(formats PRIVATE cbf_zlib)
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
