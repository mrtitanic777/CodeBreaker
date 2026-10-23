# The Guide library: the Xbox Guide's screens and behaviour (friends, messages, party, profiles, sign-in,
# achievements), shared by every port so each shows the same Guide (docs/x360/shared_guide.md).
#
# Use from a runtime's CMakeLists.txt, together with the XUI library and the live client it calls:
#   include("<CodeBreaker>/native/x360/system/guide/guide.cmake")
#   target_sources(<runtime> PRIVATE ${X360_GUIDE_SOURCES})
#   target_include_directories(<runtime> PRIVATE ${X360_GUIDE_INCLUDE_DIR})
# The sources compile as part of the runtime's target (its flags; its "log.h"); the runtime defines the functions in
# guide/host.h. Headers are included as "guide/<name>.h".

set(X360_GUIDE_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}")
file(GLOB X360_GUIDE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/guide/*.cpp")
