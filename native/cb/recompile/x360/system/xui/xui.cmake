# The XUI library: the Xbox 360's system UI (Guide, popups, virtual keyboard, achievements) drawn from XAM 17559's own
# XUI assets. Written for the Minecraft TU46 runtime (#1), shared by both runtimes (README.md).
#
# Use from a runtime's CMakeLists.txt:
#   include("<CodeBreaker>/native/x360/system/xui/xui.cmake")
#   target_sources(<runtime> PRIVATE ${X360_XUI_SOURCES})
#   target_include_directories(<runtime> PRIVATE ${X360_XUI_INCLUDE_DIR})
# The sources compile as part of the runtime's target (its flags; its "log.h"); the runtime defines the functions in
# xui/host.h. Headers are included as "xui/<name>.h".

set(X360_XUI_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}")
file(GLOB X360_XUI_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/xui/*.cpp")
