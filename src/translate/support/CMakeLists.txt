# translate/support: Python's value semantics as the C and C++17 back ends on the typed tree use them -- a value (py::V: dicts in
# order, shared containers), Python's str methods, json.loads / json.dumps and the tagged form, Python's re -- and the L2 schema
# check, built as the library cb_pysupport (included from src/CMakeLists.txt). Compiled under its own flags (not /W4 /WX).
set(PYSUPPORT_DIR "${CMAKE_CURRENT_LIST_DIR}")
add_library(cb_pysupport STATIC
  "${PYSUPPORT_DIR}/pyvalue.cc"    # a value with Python's semantics
  "${PYSUPPORT_DIR}/pystr.cc"      # Python's str methods, int(), repr()
  "${PYSUPPORT_DIR}/pyjson.cc"     # json.loads / json.dumps as Python's, and the tagged form
  "${PYSUPPORT_DIR}/pyre.cc"       # Python's re
  "${PYSUPPORT_DIR}/l2schema.cc")  # the L2 schema check
set_target_properties(cb_pysupport PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(cb_pysupport PUBLIC "${CB_NATIVE_ROOT}")   # (its headers by their path under src/: translate/support/pyvalue.hpp)
if(MSVC)
  target_compile_options(cb_pysupport PRIVATE /EHsc /bigobj /utf-8 /W3)
else()
  target_compile_options(cb_pysupport PRIVATE ${CB_FLAGS_LIB})
endif()
if(CB_ASAN)
  target_compile_options(cb_pysupport PRIVATE /fsanitize=address /Zi)
endif()
