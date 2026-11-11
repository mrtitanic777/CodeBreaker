# CodeBreaker's file-format readers in C++: formats.dll, a C interface (cbformats.h) loaded by
# codebreaker/formats/native.py through ctypes.  The executable-format readers (pe, ne, le, dos, omf, detect) are
# cb::formats (native/cb/formats, the one implementation): formats.dll only answers their questions
# (<reader>/questions.cc) from it, linking native/cb's libraries as native/cb builds them.  The other readers
# (cab, media, xbox, win, zx, lzx, xex) still live here.  `ctest` checks the library on its own, and
# native/formats/gate.py checks it against the Python readers over a corpus.
#
#   (in a vcvars64 shell)
#   cmake -S native/formats -B native/formats/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/formats/build
#   ctest --test-dir native/formats/build
cmake_minimum_required(VERSION 3.20)
project(codebreaker_formats C CXX)
set(CMAKE_CXX_STANDARD 20)              # as native/cb (docs/CPP_DESIGN.md 3.1)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# native/cb: cb_formats and what it links (cb_core, cb_xex_reader, cb_zlib), each under native/cb's own flags; only
# what formats.dll links is built.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../cb ${CMAKE_BINARY_DIR}/cb EXCLUDE_FROM_ALL)

set(FORMATS_SOURCES
  api.cc              # cbf_call: a question to its reader, the answer back
  core/bytes.cc       # bytes read with Python's rules (slices, unpack_from, find)
  core/text.cc        # Python's decoders (latin1, utf-16-le with replace, ascii)
  core/pyvalue.cc     # the values that cross the interface (a pickle subset)
  core/files.cc       # files read by path
  core/cs32.cc        # Capstone (x86-32), by the DLL Python's capstone package carries
  pe/questions.cc     # the questions of cb::formats::pe (formats/pe.py)
  ne/questions.cc     #   ne (formats/ne.py)
  le/questions.cc     #   le, the W3/W4 containers (formats/le.py)
  dos/questions.cc    #   dos: MZ, .COM, drivers, IO.SYS, packed programs (formats/dos.py)
  omf/questions.cc    #   omf (formats/omf.py)
  xex/questions.cc    # the Xbox 360 executable's header (formats/xex.py), by native/xex's reader
  core/codecs.cc      # cp437, utf-16-be, utf-8 with replace
  detect/questions.cc #   detect: what kind of file a file is (formats/detect.py)
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
  xbox/assets.cc      # Xbox assets: xsb, xbe, xzp, xdbf, ...
  xbox/graphics.cc    # Xbox textures, XPR bundles, Scaleform GFx movies, XNA content (texture, xpr, gfx, xnb)
  xbox/discs.cc       # Xbox discs: devices, XDVDFS, GoD parts, CSO / CCI, LZ4 (formats/xbox/{gdf,god,ciso,lz4block}.py)
)

# zlib 1.3.1 (MSZIP) and crc32(): native/cb's cb_zlib (the copy native/decompiler carries, core/zcrc32.c)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
# Capstone's headers, from the Python package that carries its DLL (the same version; nothing of it is linked)
if(NOT CAPSTONE_INCLUDE)
  execute_process(COMMAND ${Python3_EXECUTABLE} -c "import capstone, os; print(os.path.join(os.path.dirname(capstone.__file__), 'include'))"
                  OUTPUT_VARIABLE CAPSTONE_INCLUDE OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

# formats.dll: the questions, and native/cb's libraries they answer from (the XEX parser of native/xex is cb_xex_reader)
add_library(formats SHARED ${FORMATS_SOURCES})
target_include_directories(formats PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CAPSTONE_INCLUDE})
target_link_libraries(formats PRIVATE cb_formats cb_core cb_xex_reader cb_zlib)
if(MSVC)
  target_compile_options(formats PRIVATE /EHsc /O2 /W3 /utf-8 /Zc:char8_t-)
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
