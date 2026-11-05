# CodeBreaker's in-process DOS executive in C++: exact/emu/dos.py's run() (PSP, the MCB arena,
# program load, INT 20h/21h over a file system kept in memory or on the disk, EXEC, console, clock)
# and exact/emu/dpmi.py's DPMI host, on Unicorn's C API.  A DLL with a C interface (cbdos.h),
# loaded by codebreaker/exact/emu/native_dosexec.py through ctypes.
#
#   (in a vcvars64 shell)
#   cmake -S native/dosexec -B native/dosexec/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/dosexec/build
#
# Compiled against the headers of the Unicorn that Python's `unicorn` package carries, and links
# nothing of it: at run time it calls the very unicorn.dll Python loaded (found by path), so a run
# here executes exactly as the Python one.  UNICORN_DIR overrides where that package is.
cmake_minimum_required(VERSION 3.16)
project(codebreaker_dosexec CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
if(NOT UNICORN_DIR)
  execute_process(COMMAND ${Python3_EXECUTABLE} -c "import unicorn, os; print(os.path.dirname(unicorn.__file__))"
                  OUTPUT_VARIABLE UNICORN_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()
message(STATUS "unicorn: ${UNICORN_DIR}")

add_library(cbdos SHARED
  api.cc       # the C interface: the file system handed over, a run, its result
  uc.cc        # Unicorn, by the DLL Python loaded
  pyfmt.cc     # Python's text for what a run reports (bytes repr, %x, str.upper ...)
  fs.cc        # paths as Python makes them, the MemFS, disk files
  mcb.cc       # the DOS memory arena in guest RAM
  exec.cc      # the executive: load, INT 20h/21h, EXEC, the run loop
  dpmi.cc)     # the DPMI host
target_include_directories(cbdos PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${UNICORN_DIR}/include)
if(MSVC)
  target_compile_options(cbdos PRIVATE /EHsc /O2 /W3 /utf-8)
  target_compile_definitions(cbdos PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX UNICODE _UNICODE)
  set_property(TARGET cbdos PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")   # no VC runtime DLL needed
endif()
set_target_properties(cbdos PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
                                       LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
