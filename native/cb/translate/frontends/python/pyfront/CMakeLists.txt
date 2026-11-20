# cb/translate/frontends/python/pyfront: the Python 3 parser (source -> an AST shaped like CPython's `ast`, Python 3.14),
# the library cb_pyfront, part of native/cb's one build (included from cb/CMakeLists.txt). The translator's Python front
# end (pyast.hpp) reads programs with it. Compiled as it is, under its own flags (not /W4 /WX). Its C interface
# (pyfront.h, src/capi.cpp) is in the library too: the grammar tests and the dump tool use it.
set(PYFRONT_DIR "${CMAKE_CURRENT_LIST_DIR}")
add_library(cb_pyfront STATIC
  "${PYFRONT_DIR}/src/lexer.cpp" "${PYFRONT_DIR}/src/literals.cpp" "${PYFRONT_DIR}/src/unicode.cpp"
  "${PYFRONT_DIR}/src/parser.cpp" "${PYFRONT_DIR}/src/dump.cpp" "${PYFRONT_DIR}/src/capi.cpp")
set_target_properties(cb_pyfront PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(cb_pyfront PUBLIC "${PYFRONT_DIR}/include")
target_compile_options(cb_pyfront PRIVATE ${CB_FLAGS_LIB})
if(CB_ASAN)
  target_compile_options(cb_pyfront PRIVATE /fsanitize=address /Zi)
endif()

# pyfront_dump <file.py>: the tree as ast.dump writes it. pyfront_tests <cases>: the grammar cases (tests/cases), each
# compared with CPython's dump. Built on request.
add_executable(pyfront_dump EXCLUDE_FROM_ALL "${PYFRONT_DIR}/tools/pyfront_dump.cpp")
add_executable(pyfront_tests EXCLUDE_FROM_ALL "${PYFRONT_DIR}/tests/pyfront_tests.cpp")
foreach(t pyfront_dump pyfront_tests)
  target_link_libraries(${t} PRIVATE cb_pyfront)
  set_target_properties(${t} PROPERTIES CXX_STANDARD 20 MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  target_compile_options(${t} PRIVATE ${CB_FLAGS_LIB})
endforeach()
add_test(NAME pyfront_grammar COMMAND pyfront_tests "${PYFRONT_DIR}/tests/cases")
