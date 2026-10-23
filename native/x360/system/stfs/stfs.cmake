# The STFS package library: reads, verifies and writes Xbox 360 CON / LIVE / PIRS packages (Shared/docs/stfs.md).
#
# Use from a runtime's CMakeLists.txt:
#   include("<path to Shared>/runtime/stfs/stfs.cmake")
#   target_sources(<runtime> PRIVATE ${X360_STFS_SOURCES})
#   target_include_directories(<runtime> PRIVATE ${X360_STFS_INCLUDE_DIR})
#   target_link_libraries(<runtime> PRIVATE bcrypt)
# The sources compile as part of the runtime's target and log through its "log.h". Headers: "stfs/<name>.h".
# Standalone tests: runtime/stfs/test/build.bat.

set(X360_STFS_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}")
file(GLOB X360_STFS_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/stfs/*.cpp")
