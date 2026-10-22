# The live client: the ports' own Xbox LIVE replacement (docs/x360/network_protocol.md), shared by both runtimes.
#
# Use from a runtime's CMakeLists.txt:
#   include("<path to Shared>/runtime/live/live.cmake")
#   target_sources(<runtime> PRIVATE ${X360_LIVE_SOURCES})
#   target_include_directories(<runtime> PRIVATE ${X360_LIVE_INCLUDE_DIRS})
#   target_link_libraries(<runtime> PRIVATE ws2_32)
# The sources compile as part of the runtime's target and log through its "log.h". Headers: "live/<name>.h".

set(X360_LIVE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}" "${CMAKE_CURRENT_LIST_DIR}/third_party")
file(GLOB X360_LIVE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/live/*.cpp")
