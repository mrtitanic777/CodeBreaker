# Seamless switches between the ports' processes (native/x360/system/transition/transition/transition.h): the black
# cover window, launching with a ready event, SignalReady. A runtime includes this file and adds the sources:
#   include("<CodeBreaker>/native/x360/system/transition/transition.cmake")
#   target_sources(<runtime> PRIVATE ${X360_TRANSITION_SOURCES})
#   target_include_directories(<runtime> PRIVATE ${X360_TRANSITION_INCLUDE_DIR})
# The sources log through the runtime's "log.h" (Log), like native/x360/system/xui and live.
set(X360_TRANSITION_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(X360_TRANSITION_SOURCES "${CMAKE_CURRENT_LIST_DIR}/transition/transition.cpp")
