# The NE reader (codebreaker/formats/ne.py) as C++, written by CodeBreaker's Python front end
# (tools/py2cpp.py port codebreaker.formats.ne native/ported/ne/src), and the gate's facts program.
#
#   (in a vcvars64 shell)
#   cmake -S native/ported/ne -B native/ported/ne/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/ported/ne/build
#   python native/ported/ne/gate.py
#
# PORT_SRC points at another translation of the module (the gate's checked-int build).
cmake_minimum_required(VERSION 3.16)
project(ported_ne CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT PORT_SRC)
  set(PORT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()
add_executable(ne_facts facts.cpp ${PORT_SRC}/codebreaker/formats/ne.cpp)
target_include_directories(ne_facts PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include ${PORT_SRC})
if(MSVC)
  target_compile_options(ne_facts PRIVATE /utf-8 /EHsc /W3 /permissive- /bigobj)
endif()
