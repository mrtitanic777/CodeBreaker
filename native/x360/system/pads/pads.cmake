# PlayStation controllers (DualShock 4, DualSense) from Windows' HID for the ports' input layers (pads/pads.h).
# Use from a runtime's CMakeLists.txt:
#   include("<CodeBreaker>/native/x360/system/pads/pads.cmake")
#   target_sources(<runtime> PRIVATE ${X360_PADS_SOURCES})
#   target_include_directories(<runtime> PRIVATE "${X360_PADS_INCLUDE_DIR}")
# The sources compile as part of the runtime's target and log through its "log.h"; hid.lib and setupapi.lib are
# linked by the source (#pragma comment).
set(X360_PADS_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(X360_PADS_SOURCES "${CMAKE_CURRENT_LIST_DIR}/pads/pads.cpp")
