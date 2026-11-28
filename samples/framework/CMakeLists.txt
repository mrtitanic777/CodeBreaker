# samples/framework: the samples' shared framework (framework.h), a static library every sample links.
if(NOT TARGET cb_samples_framework)
  add_library(cb_samples_framework STATIC "${CMAKE_CURRENT_LIST_DIR}/framework.c" "${CMAKE_CURRENT_LIST_DIR}/framework.h")
  target_include_directories(cb_samples_framework PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
  target_link_libraries(cb_samples_framework PUBLIC CodeBreaker::cbsdk)
  set_target_properties(cb_samples_framework PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON)
endif()
