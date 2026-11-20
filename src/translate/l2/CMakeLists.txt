# cb/translate/l2: the L2 core library -- the L2 node model (reader, writer, schema), Python's value semantics, the C back
# end and the C++17 back end -- built as the library cb_l2, part of src's one build (included from
# cb/CMakeLists.txt). cb::translate::native_l2::Library::builtin() calls its one entry point (l2_call, include/l2api.h)
# in-process. Compiled as it is, under its own flags (not /W4 /WX).
set(L2_DIR "${CMAKE_CURRENT_LIST_DIR}")
add_library(cb_l2 STATIC
  "${L2_DIR}/src/value.cc"        # a value with Python's semantics (dicts in order, shared containers)
  "${L2_DIR}/src/pystr.cc"        # Python's str methods, int(), repr()
  "${L2_DIR}/src/json.cc"         # json.loads / json.dumps as Python's, and the tagged form
  "${L2_DIR}/src/pyre.cc"         # Python's re
  "${L2_DIR}/src/l2.cc"           # the L2 node model, from_json / to_json, the schema
  "${L2_DIR}/src/cemit.cc"        # the C back end: the pretty printer and operator stack
  "${L2_DIR}/src/cback.cc"        # ... expressions, statements, functions
  "${L2_DIR}/src/cpasses.cc"      # ... c_text's passes done on the tree
  "${L2_DIR}/src/cpasses2.cc"     # ... the locals' layout, the 32-bit and PowerPC paths' own
  "${L2_DIR}/src/cmodule.cc"      # ... what the module decides (prototypes, plans, names)
  "${L2_DIR}/src/cpp17_core.cc"   # the C++17 back end: the tree, the emitter's forms, the rules' choice
  "${L2_DIR}/src/cpp17_python.cc" # ... Python's rules
  "${L2_DIR}/src/cpp17_jmodel.cc" # ... the Java object model
  "${L2_DIR}/src/cpp17_java.cc"   # ... Java's rules
  "${L2_DIR}/src/api.cc")         # the entry point: l2_call(name, arguments) -> answer
set_target_properties(cb_l2 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(cb_l2 PUBLIC "${L2_DIR}/include" PRIVATE "${L2_DIR}/src")
target_compile_definitions(cb_l2 PRIVATE CBL2_CBACK CB_NO_FASTALLOC)   # (operator new is the program's: app/allocator_main.cpp)
if(MSVC)
  target_compile_options(cb_l2 PRIVATE /EHsc /bigobj /utf-8 /W3)
else()
  target_compile_options(cb_l2 PRIVATE ${CB_FLAGS_LIB})
endif()
if(CB_ASAN)
  target_compile_options(cb_l2 PRIVATE /fsanitize=address /Zi)
endif()

# The library's own tests (tests/), built on request: l2_tests (the node model and the C back end) and cpp17_tests.
add_executable(l2_tests EXCLUDE_FROM_ALL "${L2_DIR}/tests/test_l2.cc")
target_compile_definitions(l2_tests PRIVATE CBL2_CBACK L2_SCHEMA="${L2_DIR}/../../../config/translate/l2.schema.json"
                                            L2_TESTDATA="${L2_DIR}/tests/data")
add_executable(cpp17_tests EXCLUDE_FROM_ALL "${L2_DIR}/tests/test_cpp17.cc")
target_compile_definitions(cpp17_tests PRIVATE L2_TESTDATA="${L2_DIR}/tests/data" API_MAPS="${L2_DIR}/../../../config/java/api_maps")
foreach(t l2_tests cpp17_tests)
  target_link_libraries(${t} PRIVATE cb_l2)
  target_include_directories(${t} PRIVATE "${L2_DIR}/src")
  set_target_properties(${t} PROPERTIES CXX_STANDARD 17 MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  if(MSVC)
    target_compile_options(${t} PRIVATE /EHsc /utf-8)
    target_link_options(${t} PRIVATE /STACK:268435456)
  endif()
  add_test(NAME ${t} COMMAND ${t})
endforeach()
