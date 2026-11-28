# generate_formats.cmake -- config/formats/*.fmt as typed C++ readers (docs/FORMAT_CODEGEN.md): format_gen, built from the
# engine's own parser, writes <build>/formats_gen/formats/gen/<name>.hpp and .cpp for each description (a file only when
# its text changes), and the sources are compiled into cb_formats. Included by src/CMakeLists.txt after
# embed_formats.cmake (whose CB_FMT_FILES it reads).
add_executable(format_gen formats/format_gen_main.cpp formats/format_engine.cpp)
cb_apply_flags(format_gen)
target_link_libraries(format_gen PRIVATE cb_core)
set(CB_FMT_GEN_ROOT "${CMAKE_BINARY_DIR}/formats_gen")
set(CB_FMT_GEN_DIR "${CB_FMT_GEN_ROOT}/formats/gen")
set(CB_FMT_GEN_OUTPUTS "${CB_FMT_GEN_DIR}/registry.cpp")   # (registry.cpp: every reader's values(), for format_check --gen)
set(CB_FMT_GEN_SOURCES "${CB_FMT_GEN_DIR}/registry.cpp")
foreach(fmt IN LISTS CB_FMT_FILES)
  get_filename_component(fmt_stem "${fmt}" NAME_WE)
  list(APPEND CB_FMT_GEN_OUTPUTS "${CB_FMT_GEN_DIR}/${fmt_stem}.hpp" "${CB_FMT_GEN_DIR}/${fmt_stem}.cpp")
  list(APPEND CB_FMT_GEN_SOURCES "${CB_FMT_GEN_DIR}/${fmt_stem}.cpp")
endforeach()
# (the stamp is the command's output, the readers its byproducts: the command reruns when a description or the generator
# changes, and what includes a reader recompiles only when that reader's text changed)
add_custom_command(OUTPUT "${CB_FMT_GEN_DIR}/stamp.txt" BYPRODUCTS ${CB_FMT_GEN_OUTPUTS}
  COMMAND format_gen "${CB_FMT_GEN_DIR}" "${CMAKE_CURRENT_LIST_DIR}/../../config/formats" "${CB_FMT_GEN_DIR}/stamp.txt"
  DEPENDS format_gen ${CB_FMT_FILES}
  COMMENT "format_gen: config/formats/*.fmt as C++ readers" VERBATIM)
add_custom_target(cb_formats_gen DEPENDS "${CB_FMT_GEN_DIR}/stamp.txt")
target_sources(cb_formats PRIVATE ${CB_FMT_GEN_SOURCES})
target_include_directories(cb_formats PUBLIC "${CB_FMT_GEN_ROOT}")
add_dependencies(cb_formats cb_formats_gen)
